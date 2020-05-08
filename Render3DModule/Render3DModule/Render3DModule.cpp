#include "stdlib.h"

#include <cassert>
#include <iterator>
#include <comdef.h>
#include <set>
#include <ctime>

#include "Help.h"
#include "Config.h"
#include "ObjHolder.h"
#include "ObjAnalysis.h"
#include "XmlCreator.h"
#include "ExrRender.h"
#include "TargetRender.h"
#include "Initializer.h"
#include "LabelChanger.h"

#define LOG_MAX_SIZE 1048576
#define LOG_MAX_NUMB 4

IniAttributes iniAttributes;
static std::set<ObjBaseInfo> objsBaseInfo;

std::shared_ptr<spdlog::logger> my_logger;
std::string renderDataRootPath;
Statistics statistics;
ObjStatistic currentObjStatistic;
std::string logFileStatistics;
std::string separator;

clock_t startTime, endTime;

void TEST_CreateRenderPngByMitsuba(std::string renderDataRootPath);
void CreateRenderPngByMitsuba(std::string renderDataRootPath);

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cout << "INFO: please input only 1 parameter: data path for rendering! " << std::endl;
		return -1;
	}
	else
	{
		startTime = clock();
		Initializer initializer;
		initializer.ReadIniFile();
		iniAttributes = initializer.GetIniAttributes();
		objsBaseInfo = initializer.GetIniObjsBaseInfo();

		//std::cout
		//	<< iniAttributes.debugLogDetails << std::endl
		//	<< iniAttributes.performanceNeedSleep << std::endl
		//	<< iniAttributes.performanceSleepSecond << std::endl
		//	<< iniAttributes.objBaseScale << std::endl
		//	<< iniAttributes.xmlBatchCount << std::endl
		//	<< iniAttributes.exrBatchCount << std::endl
		//	<< iniAttributes.debugDeleteExrFile << std::endl
		//	<< iniAttributes.debugIncreaseXml << std::endl
		//	<< iniAttributes.stepScale << std::endl
		//	<< iniAttributes.stepAngle << std::endl
		//	<< iniAttributes.stepEmitter << std::endl
		//	<< iniAttributes.statisticsMaxObjIDSize << std::endl;

		std::cout << "INFO: working Path: " << argv[1] << std::endl;

		renderDataRootPath = argv[1];
		std::string logFilePath = renderDataRootPath + path_log;
		logFileStatistics = logFilePath + log_file_statistics;
		Help::CreateFolder(logFilePath);

		statistics.SetStatisticslogFile(logFileStatistics);
		statistics.PrintFileStructure(argv[1], "before rendering");

		log_file_basename = logFilePath + log_file_basename;
		my_logger = spdlog::rotating_logger_mt("main", log_file_basename, LOG_MAX_SIZE, LOG_MAX_NUMB);
		my_logger->info("===================================================================================");
		my_logger->info("============================  Start to Render Objects  ============================");
		my_logger->info("===================================================================================");
		my_logger->info("=>[begin to render in path: {}]", argv[1]);

		//TEST_CreateRenderPngByMitsuba(renderDataRootPath);
		CreateRenderPngByMitsuba(renderDataRootPath);
		my_logger->flush();

		statistics.PrintFileStructure(argv[1], "after rendering");
		endTime = clock();
		my_logger->info("Done. ^_^");

		// 13) print statistics
		double time = (double)(endTime - startTime) / (CLOCKS_PER_SEC*60);
		StatisticsSummarizeParam statisticsParam;
		statisticsParam.timeConsumption = time;
		std::cout << "The run time is: " << time << " minutes" << std::endl;
		my_logger->info("========== Run Time: {} minutes. ==========", time);
		statistics.PrintSummarize(statisticsParam);

		my_logger->flush();

	}
	return 0;
}
void CreateRenderPngByMitsuba(std::string renderDataRootPath)
{

	std::string objsPath = renderDataRootPath + path_src_objs;
	std::string labelsPath = renderDataRootPath + path_src_labels;
	std::string xmlsPath = renderDataRootPath + path_dst_xmls;
	std::string targetsPath = renderDataRootPath + path_dst_png;

	//--------1.换label----------------------------//
	// 1) 准备label: 分割标签成单个形式的
	// 2) 准备texture: 单个形式的label 贴合到 obj 的纹理上
	// 3) 若有必要保存原始纹理, 复制原始纹理图片到对应obj文件夹下texture文件夹下即可
	//		texture 文件夹命名规范: "objID_texture"
	//(注:自动换需要考虑：标签放缩/贴的位置)

	LabelChanger labelChanger;
	labelChanger.Setlabels(labelsPath);
	labelChanger.SetXmlsPath(xmlsPath);

	//1.1 获取标签路径 ...\src_obj\label\
	//1.2 针对每一个obj 获取每个obj的texture ...\src_obj\obj\14042_750_mL_Wine_Bottle_r_v1_L3\1.jpg
	//1.3 针对每一个obj 创建 obj_texture 路径
	//1.3 针对每一个obj 合成新的texture, 存入 obj_texture 路径


	//--------2.对每个obj生产对应的xml渲染文件组-------//
	// ObjHolder
	// 0) 获取待处理文件终的objID存起来	
	// 1) 遍历src_obj\obj 目录, 获得所有obj对象ID

	ObjHolder objHolder;
	objHolder.SetObjs(objsPath);

	std::vector<std::string> objIDs = objHolder.GetObjs();
	my_logger->info("=>[STATISTICS]  total number of objects: [{}]", objIDs.size());
	my_logger->flush();

	ObjAnalysis objAnalysis;
	ParameterXml paraXml;

	size_t xmlcounter(0);
	XmlCreator xmlCreator;
	xmlCreator.SetXmlsPath(xmlsPath);

	std::string objXmlsPath;
	std::string objExrsPath;
	std::vector<std::string> objXmlPathIDs;

	ExrRender exrRender;
	exrRender.SetXmlsPath(xmlsPath);

	TargetRender targetRender;
	targetRender.SetTargetsPath(targetsPath);
	Help::CreateFolder(targetsPath);

	size_t textureCounter(0);
	std::string currentTexture;
	std::set<ObjBaseInfo>::iterator it;
	ObjBaseInfo objBaseInfo;

	for (size_t i = 0; i < objIDs.size(); ++i)
	{
		my_logger->info("=>[NO.{0}] [{1}]", i + 1, objIDs[i]);
		currentObjStatistic.objID = objIDs[i];

		// ObjAnalysis
		// 2) 获取每一个obj 的 x y z 的 max 与 min, 
		// 3) 函数计算obj平移向量, 放缩基数,对应纹理名称
		//OK std::cout << objsPath << '\' << objIDs[i] << std::endl;

		objAnalysis.SetObj(objsPath, objIDs[i]);
		objAnalysis.Calculate();

		// set base rotate and base translate of x/y/z by obj analysis
		//paraXml.Trans.x = (int)objAnalysis.GetShiftX();
		//paraXml.Trans.y = (int)objAnalysis.GetShiftY();
		//paraXml.Trans.z = (int)objAnalysis.GetShiftZ();
		//paraXml.baseObjScale = objAnalysis.GetScale();
		//my_logger->info("->[{0}]: shift(x,y,z)=({1:d},{2:d},{3:d}) baseObjScale={4:03.2f}",
		//	objIDs[i], paraXml.Trans.x, paraXml.Trans.y, paraXml.Trans.z, paraXml.baseObjScale);

		labelChanger.SetObj(objIDs[i]);
		labelChanger.SetSrcTexturePathID(objAnalysis.GetTexturePathID());

		// set base rotate and base translate of x/y/z by obj ini object basic info file
		it = std::find_if(objsBaseInfo.cbegin(), objsBaseInfo.cend(), ObjBaseInfoFinder(objIDs[i]));
		if (it != objsBaseInfo.cend())
		{
			objBaseInfo = *it;
		}

		Roi objRoi;
		objRoi.leftTopX = objBaseInfo.Roi.leftTopX;
		objRoi.leftTopY = objBaseInfo.Roi.leftTopY;
		objRoi.rightBottomX = objBaseInfo.Roi.rightBottomX;
		objRoi.rightBottomY = objBaseInfo.Roi.rightBottomY;
		my_logger->info("->[{}]: objRoi(leftTopX,leftTopY,rightBottomX,rightBottomY)=({},{},{},{})",
			objIDs[i], objRoi.leftTopX, objRoi.leftTopY, objRoi.rightBottomX, objRoi.rightBottomY);

		labelChanger.SetObjRoi(objRoi);
		labelChanger.Start();

		paraXml.baseObjScale = objBaseInfo.baseInfo.objBaseScale;
		paraXml.Rotat.x = objBaseInfo.baseInfo.objBaseRotateX;
		paraXml.Rotat.y = objBaseInfo.baseInfo.objBaseRotateY;
		paraXml.Rotat.z = objBaseInfo.baseInfo.objBaseRotateZ;
		paraXml.Trans.x = objBaseInfo.baseInfo.objBaseTranslateX;
		paraXml.Trans.y = objBaseInfo.baseInfo.objBaseTranslateY;
		paraXml.Trans.z = objBaseInfo.baseInfo.objBaseTranslateZ;
		my_logger->info("->[{}]: baseObjScale={:03.2f} rotate(x,y,z)=({},{},{}) shift(x,y,z)=({},{},{})",
			objIDs[i], paraXml.baseObjScale, paraXml.Rotat.x, paraXml.Rotat.y, paraXml.Rotat.z,
			paraXml.Trans.x, paraXml.Trans.y, paraXml.Trans.z);
		my_logger->flush();

		//XmlCreator
		// 4) 针对每个obj,复制template并加名称为" *_n.xml ",到指定ObjID对应路径
		// 5) 针对每个obj,循环变化条件 modify 变动参数(), 到指定ObjID对应路径
		xmlCreator.SetObjID(objIDs[i]);
		xmlCreator.SetObjPathID(objAnalysis.GetObjPathID());
		xmlCreator.SetObjParameter(paraXml);
		xmlcounter = xmlCreator.Start();
		currentObjStatistic.xmlNumber = xmlcounter;
		my_logger->info("    |- [STATISTICS] [{0}]: now has [{1}] xml files.", objIDs[i], xmlcounter);
		my_logger->flush();

		//--------3.对每个obj文件，渲染生产png------------//
		//渲染字符串: mitsuba.exe -o F:\test\1.exr F:\test\1.xml //指定文件夹时只能一个个渲染,不能批处理!
		// 6) 针对每个obj,换一个texture,执行一组渲染(xml->exr->png),生产一组png
		// 7) 渲染成功后, 删除对应1个obj的xml路径下的.exr文件,以减小磁盘占用率.

		//ExrRender: 生成exr,存入对应obj的exr文件夹下
		//hereto:
		// 8) 获取texture数量, 复制一个新的texture到 obj 目录中,替换掉原有 texture 图像
		// 9) 循环teture数量,执行渲染: 
		//		批处理渲染:	获取所以文件xml文件名称存入vector中,
		//					每10个 批处理一次,生成.exr文件
		//		移动到 对应obj的exr文件夹下

		objXmlsPath = xmlsPath + objIDs[i] + "_xml\\";
		objExrsPath = xmlsPath + objIDs[i] + "_exr\\";

		//Help::Getfiles(objXmlPathIDs, objXmlsPath, false, true);
		Help::GetSpecifiedTypefiles(objXmlPathIDs, objXmlsPath, "xml", true);
		Help::CreateFolder(objExrsPath);

		exrRender.SetobjExrsPath(objExrsPath);
		exrRender.SetObjXmlPathIDs(objXmlPathIDs);

		exrRender.SetObjID(objIDs[i]);
		textureCounter = exrRender.CountTexture();

		my_logger->info("    |- [STATISTICS] [{0}]: now has [{1}] texture files.", objIDs[i], textureCounter);

		currentObjStatistic.textureNumber = textureCounter;
		currentObjStatistic.expectedSum = textureCounter * xmlcounter;

		exrRender.SetOldTexture(objAnalysis.GetTexturePathID());
		for (size_t i = 0; i < textureCounter; ++i)
		{
			exrRender.UpdateTexture(i);
			exrRender.Start(i);
		}
		my_logger->flush();

		//TargetRender: 
		// 10)	获取所以文件exr文件名称存入vector中,批处理一次 生成.png文件
		// 11)	移动到 对应dst_png文件夹下
		targetRender.SetObjID(objIDs[i]);

		targetRender.SetobjExrsPath(objExrsPath);
		targetRender.Start();
		currentObjStatistic.isOk = (currentObjStatistic.expectedSum == currentObjStatistic.actualSum);
		statistics.AddObjStatistic(currentObjStatistic);

		//TO-DO: PostProcessor: 渲染成功后, 删除对应1个obj的xml路径下的.exr文件,以减小磁盘占用率.
		// 12) 若要 节省内存, 删除 exr 文件夹
		//	   PostProcessor::start();
	}

}

void TEST_CreateRenderPngByMitsuba(std::string renderDataRootPath)
{

	//1.换label
	//	1) 准备label: 分割标签成单个形式的
	//	2) 准备texture: 单个形式的label 贴合到 obj 的纹理上
	//(注:自动换需要考虑：标签放缩/贴的位置)

	//2.对每个obj生产对应的xml渲染文件组

	//3.对每个obj文件，换一个texture,执行一组渲染,生产一组png
	//GetModuleFileName(NULL, exeFullPath, MAX_PATH);

	std::string cmdCreateExr;
	std::string cmdCreatePng;

	std::string objFullNameIn;
	std::string pngFullNameOut;


	//获取文件名
	std::string objID = "14042_750_mL_Wine_Bottle_r_v1_L3";
	objFullNameIn = renderDataRootPath + path_dst_xmls + objID + "_xml" + "\\" + objID; // NOT TO-DO: +i
	pngFullNameOut = renderDataRootPath + path_dst_png + objID; // TO-DO: +i
	//拼接命令
	cmdCreateExr = createExrByMitsuba + objFullNameIn + ".xml";
	std::cout << cmdCreateExr.c_str() << std::endl;

	cmdCreatePng = createPngByMitsuba + "-o " + pngFullNameOut + ".png " + objFullNameIn + ".exr";
	std::cout << cmdCreatePng.c_str() << std::endl;

	//执行渲染，生成exr
	my_logger->info("Begin to create EXR file for {}", objID);
	system(cmdCreateExr.c_str());
	//system("mitsuba  F:\\study3D\\align_texture\\RenderProject\\dst_xml\\14042_750_mL_Wine_Bottle_r_v1_L3.xml");

	my_logger->info("Begin to create PNG file for {}", objID);
	//格式转换，生成png
	system(cmdCreatePng.c_str());
	//system("mtsutil tonemap -o F:\\study3D\\align_texture\\RenderProject\\dst_xml\\14042_750_mL_Wine_Bottle_r_v1_L3.png  F:\\study3D\\align_texture\\RenderProject\\dst_xml\\14042_750_mL_Wine_Bottle_r_v1_L3.exr");
}