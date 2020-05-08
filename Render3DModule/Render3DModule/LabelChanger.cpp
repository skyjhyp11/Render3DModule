#include "LabelChanger.h"

#include <core/core.hpp>  
#include <highgui/highgui.hpp>  
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

LabelChanger::LabelChanger()
{
}


LabelChanger::~LabelChanger()
{
}

void LabelChanger::Setlabels(const std::string & labelsPath)
{
	//读取labels文件 存入vector中
	Help::Getfiles(m_labels, labelsPath, false, true);

	//Help::GetSpecifiedTypefiles(m_labels, labelsPath, "png", true);
	//Help::GetSpecifiedTypefiles(m_labels, labelsPath, "jpg", true);

}

void LabelChanger::SetSrcTexturePathID(const std::string & srcTexturePathID)
{
	m_objSrcTexturePathID = srcTexturePathID;
}

void LabelChanger::SetObj(const std::string & objID)
{
	m_objID = objID;
}

void LabelChanger::SetXmlsPath(const std::string & xmlsPath)
{
	m_xmlsPath = xmlsPath;
}

void LabelChanger::SetObjRoi(const Roi & objRoi)
{
	m_objRoi = objRoi;
}

void LabelChanger::Start()
{
	//1. 创建obj_texture路径, 不存在创建, 存在清空
	m_objDstTexturesPath = m_xmlsPath + m_objID + "_texture\\";
	if (Help::CreateFolder(m_objDstTexturesPath) == 0)
	{
		Help::ClearFolder(m_objDstTexturesPath);
	}

	//2. 循环 label 数量, 合成新的texture, 存入obj_texture路径
	std::string srcTextureFormat = m_objSrcTexturePathID.substr(m_objSrcTexturePathID.find_last_of("."));
	cv::Mat srcTexture = cv::imread(m_objSrcTexturePathID);
	cv::Mat tmpTexture;
	cv::Mat currentlabel;
	cv::Mat tmplabel;

	cv::Rect roi_rect;
	cv::Size roiSize;
	int dx = m_objRoi.rightBottomX - m_objRoi.leftTopX;
	int dy = m_objRoi.rightBottomY - m_objRoi.leftTopY;
	if (dx > 0 && dy > 0)
	{
		roiSize.width = dx;
		roiSize.height = dy;
	}
	else
	{
		std::cout << "ERR: objRoi is incorrect! Please check file \"object_basic_info.ini\"! " << std::endl;
		assert(false);
	}

	std::string outputFile = m_objDstTexturesPath;
	//cv::imshow("srcTexture", srcTexture);
	//cv::waitKey(6000);

	if (srcTexture.empty())
	{
		std::cout << "ERR: read obj source texture failed!!!" << std::endl;
		assert(false);
	}

	//here to 将图片变成纯黑色的！
	if (iniAttributes.createMaskModule)
	{
		srcTexture.setTo(0);
	}

	size_t i = 0;
	my_logger->info("->[{}]: begin to create new textures for this object in path {}", m_objID, m_objDstTexturesPath);

	for (i = 0; i < m_labels.size(); ++i)
	{
		tmpTexture = srcTexture;
		currentlabel = cv::imread(m_labels[i]);
		cv::resize(currentlabel, tmplabel, roiSize);
		if (iniAttributes.createMaskModule)
		{
			cv::cvtColor(tmplabel, tmplabel, CV_BGR2GRAY);
			//GaussianBlur(tmplabel, tmplabel, cv::Size(15, 15), 0, 0);
			cv::threshold(tmplabel, tmplabel, 125, 255, CV_THRESH_BINARY);
			//cv::imshow("tmplabel", tmplabel);
			//cv::waitKey(6000);
			cv::cvtColor(tmplabel, tmplabel, CV_GRAY2BGR);

		}

		roi_rect = cv::Rect(m_objRoi.leftTopX, m_objRoi.leftTopY, tmplabel.cols, tmplabel.rows);
		tmplabel.copyTo(tmpTexture(roi_rect));

		outputFile = m_objDstTexturesPath + std::to_string(i + 1) + srcTextureFormat;
		cv::imwrite(outputFile, tmpTexture);
	}

}
