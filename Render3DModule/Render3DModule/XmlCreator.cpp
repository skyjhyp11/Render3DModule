#include "XmlCreator.h"

//////////////////////////////////////////////
//   The smaller the steps are,             //
//   the more number xml files will create. //
//STEP_ANGLE = 45 is good to see the effect //
#define STEP_SCALE		0.05
#define STEP_ANGLE		45
#define STEP_EMITTER	0.5
/////////////////////////////////////////////

#define DEBUG_INCREASE_XML FALSE

XmlCreator::XmlCreator()
{
}

XmlCreator::~XmlCreator()
{
}

void XmlCreator::SetXmlsPath(const std::string &xmlsPath)
{
	m_xmlsPath = xmlsPath;
}

void XmlCreator::SetObjID(const std::string &objID)
{
	m_objID = objID;
}

void XmlCreator::SetObjPathID(const std::string &objPathID)
{
	m_objPathID = objPathID;
}

void XmlCreator::SetObjParameter(const ParameterXml &paraXml)
{
	m_paraXml = paraXml;
}

std::string XmlCreator::GetObjXmlPath() const
{
	return m_objXmlPath;
}

size_t XmlCreator::Start()
{
	if (m_xmlsPath.empty() || m_objID.empty())
	{
		my_logger->error("    xml Path or object is not correct");
		return 0;
	}

	//1.创建objID目录 objID_xml
	std::string folderName = m_xmlsPath + m_objID + "_xml";
	m_objXmlPath = folderName + "\\";

	Help::CreateFolder(folderName);

	//在上述目录中,开始循环创建xml文件
	//2. 循环层次 大小 -> 角度 -> 光照
	//3.	复制temple.xml到xml目录,命名objID_n.xml
	//4.	更新一次obj文件名、 translate 值
	//5.	循环更新 大小、角度、光照

	std::string templateXmlPath = m_xmlsPath + "template.xml";
	std::string baseXmlFileName = m_objXmlPath + m_objID + "_";
	std::string xmlFileName;

	size_t xmlFileNumber = 0;
	ParameterXmlModify parameterXmlModify;

	my_logger->info("  [{}]: current obj \"{}\"", m_objID, m_objPathID);
	my_logger->info("  [{}]: begin to create xmls \"{}*.xml\"", m_objID, baseXmlFileName);

	// modify scale value of object
	float valueObjScale = m_paraXml.baseObjScale;
	int valueZ = 0;
	int valueY = 0;
	int valueX = 0;
	float valueEmitScale = 1;
	float valueEmitPointPositionX = 0;

#if DEBUG_INCREASE_XML
	for (valueObjScale = m_paraXml.baseObjScale - (float)iniAttributes.stepScale; //STEP_SCALE
		valueObjScale <= m_paraXml.baseObjScale + (float)iniAttributes.stepScale;
		valueObjScale += (float)iniAttributes.stepScale)
	{
		//rotate x -need have
		//for (valueX = -30; valueX <= 30; valueX += iniAttributes.stepAngle) // STEP_ANGLE
		for (valueX = -30; valueX <= 30; valueX += 30) //STEP_ANGLE
		{
#endif		

			//rotate y
			//for (valueY = -30; valueY <= 30; valueY += iniAttributes.stepAngle) //STEP_ANGLE
			//for (valueY = -10; valueY <= 10; valueY += 10) //STEP_ANGLE
			{
				//rotate z
				for (valueZ = -30; valueZ <= 30; valueZ += iniAttributes.stepAngle) //STEP_ANGLE
				{
					//modify emitter
					//for (float valueEmitScale = 0.5; valueEmitScale <= 1.5; 
					//for (valueEmitScale = 0.5; valueEmitScale < 1.5;// <=1.5
					//	valueEmitScale += (float)iniAttributes.stepEmitter) //STEP_EMITTER
					for (valueEmitPointPositionX = -2; valueEmitPointPositionX < 2; // need have <=2
						valueEmitPointPositionX += 2)
					{
						xmlFileName = baseXmlFileName;
						xmlFileName.append(std::to_string(xmlFileNumber + 1)).append(".xml");
						Help::FileCopy(templateXmlPath, xmlFileName);
						++xmlFileNumber;
						parameterXmlModify.index = xmlFileNumber;
						parameterXmlModify.Rotat.x = valueX;
						parameterXmlModify.Rotat.y = valueY;
						parameterXmlModify.Rotat.z = valueZ;
						//parameterXmlModify.objScale = valueObjScale;
						parameterXmlModify.emitScale = valueEmitScale;
						parameterXmlModify.emitPointPositionX = valueEmitPointPositionX;
						ModifySeceneXmlFile(xmlFileName, parameterXmlModify);
					}
				}
			}
#if DEBUG_INCREASE_XML			
		}
	}
#endif
	my_logger->flush();
	return xmlFileNumber;
}

void XmlCreator::ModifySeceneXmlFile(const std::string &xmlFileName, const ParameterXmlModify &para)
{
	TiXmlDocument doc(xmlFileName.c_str());
	if (!doc.LoadFile()) return;

	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem;
	TiXmlHandle hRoot(0);
	TiXmlHandle subRoot(0);
	TiXmlHandle subsubRoot(0);

	pElem = hDoc.FirstChildElement().Element();
	if (!pElem) return;

	hRoot = TiXmlHandle(pElem);

	// To one specified object, all xml files should have the same objPathID 
	// and the same translate value, but should be not identical in different 
	// about obj-transform-scale, obj-transform-rotate, and emitter-scale.

	//// modify value of shape
	pElem = hRoot.FirstChild("shape").Element();
	subRoot = TiXmlHandle(pElem);

	// modify value of shape-string. i.e. the specified object
	pElem = subRoot.FirstChild("string").Element();
	pElem->SetAttribute("value", m_objPathID.c_str());

	// modify value of shape-transform. i.e. the shift value of specified object
	pElem = subRoot.FirstChild("transform").Element();
	subsubRoot = TiXmlHandle(pElem);

	//modify value of shape-transform-scale
	pElem = subsubRoot.FirstChild("scale").Element();
	pElem->SetDoubleAttribute("value", m_paraXml.baseObjScale); //para.objScale

	//modify value of shape-transform-rotate
	pElem = subsubRoot.FirstChild("rotate").Element();
	pElem->SetAttribute("angle", m_paraXml.Rotat.z);
	pElem = pElem->NextSiblingElement();
	pElem->SetAttribute("angle", m_paraXml.Rotat.x);
	pElem = pElem->NextSiblingElement();
	pElem->SetAttribute("angle", m_paraXml.Rotat.y);

	//modify value of shape-transform-translate
	pElem = subsubRoot.FirstChild("translate").Element();
	pElem->SetDoubleAttribute("x", m_paraXml.Trans.x);
	pElem->SetDoubleAttribute("y", m_paraXml.Trans.y);
	pElem->SetDoubleAttribute("z", m_paraXml.Trans.z);

	// loop  modify value of shape-transform-x/y/z
	pElem = pElem->NextSiblingElement();
	pElem->SetAttribute("angle", para.Rotat.z);
	pElem = pElem->NextSiblingElement();
	pElem->SetAttribute("angle", para.Rotat.x);
	pElem = pElem->NextSiblingElement();
	pElem->SetAttribute("angle", para.Rotat.y);
	if (iniAttributes.createMaskModule == true)
	{
		//// modify value of scene-emitter(envmap)-float
		pElem = hRoot.FirstChild("emitter").FirstChild("string").Element();
		pElem->SetAttribute("value", envmap_mask.c_str());
	}

	//// modify value of scene-emitter(envmap)-float
	pElem = hRoot.FirstChild("emitter").FirstChild("float").Element();
	pElem->SetDoubleAttribute("value", para.emitScale);

	//// modify value of scene-emitter(point)-point
	pElem = hRoot.FirstChild("emitter").Element()->NextSiblingElement();
	subRoot = TiXmlHandle(pElem);
	pElem = subRoot.FirstChild("point").Element();
	pElem->SetDoubleAttribute("x", para.emitPointPositionX);


	doc.SaveFile(xmlFileName.c_str());

	if (iniAttributes.debugLogDetails) //DEBUG_LOG_DETAILS
		my_logger->info("    |* modify_{:5d}: rotate: ({:3d},{:3d},{:3d}), emitter of envmap: ({:03.2f}), emitter of point-x: ({:03.2f})",
			para.index, para.Rotat.x, para.Rotat.y, para.Rotat.z, para.emitScale, para.emitPointPositionX);

}
