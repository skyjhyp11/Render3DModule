#pragma once
#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <string>

//static std::string pathDstXml("F:\\AlignTexture\\RenderProject\\dst_xml\\");
//static std::string pathSrcObj("F:\\AlignTexture\\RenderProject\\src_obj\\");

//all names with "path" means having "\\" as postfix in codes.
static std::string path_src_objs("\\src_obj\\obj\\");
static std::string path_src_labels("\\src_obj\\label\\");
static std::string path_dst_xmls("\\dst_xml\\");
static std::string path_dst_png("\\dst_png\\");
static std::string path_log("\\log\\");

static std::string createExrByMitsuba("mitsuba");
static std::string createPngByMitsuba("mtsutil tonemap");
static std::string cmd;

static std::string log_file_basename("render_3d_module.log");
static std::string log_file_statistics("statistics.log");
static std::string ini_file_config("config.ini");
static std::string ini_file_object_basic_info("object_basic_info.ini");

static std::string envmap_background("..\\room.exr");
static std::string envmap_mask("..\\mask_black.exr");


#endif /*__CONFIG_H__*/
