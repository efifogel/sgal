// Copyright (c) 2004 Israel.
// All rights reserved.
//
// This file is part of SGAL; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; version 2.1 of the
// License. See the file LICENSE.LGPL distributed with SGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the
// software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING
// THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE.
//
// $Source$
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

#include <direct.h>
#include <stdlib.h>

#include "SGAL/basic.hpp"
#include "Socket_handler.h"
#include "Movie_recorder.h"
#include "Scene_graph.h"
#include "Local_stream.h"
#include "Os.h"
#include "Field_infos.h"
#include "Message_box.h"
#include "Scene_graph.h"
#include "Element.h"
#include "Container_proto.h"
#include "Trace.h"

SGAL_BEGIN_NAMESPACE

std::string Movie_recorder::s_tag = "enbMovieRecorder";
Container_proto * Movie_recorder::s_prototype = 0;

// Default values: */
const char * Movie_recorder::m_defMovieFileName = "movie";
const char * Movie_recorder::m_defHintFileName = "hint";
const Int Movie_recorder::m_defTotalFrames = 160;
const Short Movie_recorder::m_defNumLevels = 4;
const int Movie_recorder::m_defWidth = 352;
const int Movie_recorder::m_defHeight = 288;
const ERecMode Movie_recorder::m_defMode = rmFile;

REGISTER_TO_FACTORY(Movie_recorder, "Movie_recorder");

#define BLOCK_SIZE  16

#define LOW_Q    8
#define STEP     2

static int table3[][2][2] = {
  {{2, 2}, {0, 0}  },
  {{2, 0}, {-2, -2}},
  {{0, -2},{-4, -2}}};

static int table4[][2][2][2] = {
  {{{2, 2}, {2, 2}}, {{0, 0}, {0, 0}}},
  {{{2, 2}, {0, 0}}, {{-2, -2}, {-2, -2}}},
  {{{2, 0}, {-2, -2}}, {{-4, -4}, {-4, -4}}},
  {{{0, -2},{-4, -6}}, {{-6, -6},{-6, -6}}}};

static int quants[][2][2][2] = {
  {{{4, 4}, {4, 4}}, {{2, 2}, {2, 2}}},
  {{{6, 6}, {4, 4}}, {{2, 2}, {2, 2}}},
  {{{8, 6}, {4, 4}}, {{2, 2}, {2, 2}}},
  {{{8, 6},{4, 4}}, {{2, 2},{2, 2}}}};

  /*! Constructor */
Movie_recorder::Movie_recorder(Boolean proto) :
  Node(proto),
  m_movieFileName(m_defMovieFileName), 
  m_hintFileName(m_defHintFileName), 
  m_isEnabled(SGAL_FALSE),
  m_numLevels(m_defNumLevels),
  m_frameNo(0),
  m_totalFrames(m_defTotalFrames),
  m_context(0),
  m_sceneGraph(0),
  m_pixels(0),
  m_rgbBuffer(0),
  m_rgbBuffer2(0),
  m_hintBuffer(0),
  m_bgBuffer(0),
  m_quantBuffer(0),
  m_width(m_defWidth),
  m_height(m_defHeight),
  m_mode(m_defMode)
{
  m_hintType = "none";
  m_horzBlocks = m_width/BLOCK_SIZE;
  m_vertBlocks = m_height/BLOCK_SIZE;
}

/*! Destructor */
Movie_recorder::~Movie_recorder()
{
  TRACE_MSG(Trace::DESTRUCTOR, "~Movie_recorder ...");
  DELETE_ARRAY(m_pixels);
  DELETE_ARRAY(m_rgbBuffer);
  DELETE_ARRAY(m_rgbBuffer2);
  DELETE_ARRAY(m_hintBuffer);
  DELETE_ARRAY(m_bgBuffer);
  DELETE_ARRAY(m_quantBuffer);
  TRACE_MSG(Trace::DESTRUCTOR, " completed\n");
}

/**
 * Set the snapshot's file name to fileName.
 */
void Movie_recorder::set_movieFileName(String filename)
{
  m_movieFileName = filename;
}

/**
 * Set the name of the file to store the hint frames.
 */
void Movie_recorder::SetHintFileName(String filename)
{
  m_hintFileName = filename;
}

void Movie_recorder::SetTotalFrames(int num) 
{
  m_totalFrames = num;
}

void Movie_recorder::SetRecordingMode(const String mode)
{
  if (mode == "socket") {
    m_mode = rmSocket;
  } else {
    m_mode = rmFile;
  }
}

/**
 * SetTrigger - if isTriggered = TRUE, make a ppm image file.
 */
void Movie_recorder::set_enabled(Field_info* field_info)
{

  m_isEnabled = SGAL_TRUE;
  m_frameNo = 0;

  if (!m_pixels) {
    m_pixels = new UChar[3*m_width*m_height];
    m_rgbBuffer = new UChar[3*m_width*m_height];
    m_rgbBuffer2 = new UChar[3*m_width*m_height];
    m_hintBuffer = new UChar[m_width/BLOCK_SIZE*m_height/BLOCK_SIZE];
    m_bgBuffer = new short[m_width/BLOCK_SIZE*m_height/BLOCK_SIZE];
    m_quantBuffer = new short[m_width/BLOCK_SIZE*m_height/BLOCK_SIZE];
  }

  if (m_mode == rmSocket) 
  {
    if (m_mode == rmSocket) {
      Start();
      m_status = "Waiting for connection...";
      m_status = "Connected...";

    }
  } 
  else 
  {
    
    String folder = EOperatingSystem::get_instance()->get_snapshotsFolder();
    m_movieFullName = folder + "/" + m_movieFileName + ".enm";
    m_hintFullName = folder + "/" + m_hintFileName + ".enh";

    // this is to create the files that are later opened in Append mode
    ELocalStream stream(m_movieFullName);
    stream.Open(EStream::EOpenMode::omWrite,  EStream::EOpenType::otBinary);
    stream.Close();
    ELocalStream hintStream(m_hintFullName);
    hintStream.Open(EStream::EOpenMode::omWrite,  EStream::EOpenType::otBinary);
    hintStream.Close();

    // this is for debuging purposes only
    if (m_totalFrames == 1) {
      ELocalStream test_stream(folder + "/" + m_hintFileName + "_quant.pgm");
      test_stream.Open(EStream::EOpenMode::omWrite,  EStream::EOpenType::otBinary);
      test_stream.Close();

      ELocalStream bg_stream(folder + "/" + m_hintFileName + "_bg.pgm");
      bg_stream.Open(EStream::EOpenMode::omWrite,  EStream::EOpenType::otBinary);
      bg_stream.Close();
    }
  }

}

bool Movie_recorder::UpdateTime()
{
  if (!m_isEnabled || !m_context) {
    return true;
  }

  // if the viewport is not initialized, return here
  int x, y, width, height;
  m_context->get_viewport(x, y, width, height);
  if (width == 0) {
    return 0;
  }


  ProcessFrame();

  if (m_mode == rmSocket) 
  {
    bool res = Reply((char*)m_rgbBuffer, 3*m_width*m_height);
    if (res) {
      m_status = "Sending data...";
    } else {
      m_status = "Disconnecting...";
    }
  } else 
  {
    WriteBuffers();
  }

  // if this is the last frame, stop the recording
  m_frameNo++;
  if ((m_mode == rmFile) && (m_frameNo == m_totalFrames)) {
    m_isEnabled = SGAL_FALSE;
  }


  return true;
}

bool Movie_recorder::ProcessFrame()
{
  ULong written=0;

  // read the rgb values from the frame buffer
  glReadBuffer(GL_FRONT);
  glReadPixels(0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, m_rgbBuffer2);
  glReadBuffer(GL_BACK);
  // turn picture up-side-down
  for (int i = 0 ; i < m_height ; i++) 
  {
    memcpy(&m_rgbBuffer[m_width*3*(m_height-i-1)], &m_rgbBuffer2[m_width*3*i], m_width*3);
  }

  

  // draw and then read the background hint frame from the color buffer
  m_sceneGraph->Record();
  glReadPixels(0, 0, m_width, m_height, GL_RED, GL_UNSIGNED_BYTE, m_pixels);



  // set the values of background for each block. If at least one pixel 
  // in the block is FG, the block is set to be Foreground bloack.
  int k = 0;
  for (int row = m_vertBlocks-1 ; row >= 0 ; row--) {
    for (int col = 0 ; col < m_horzBlocks ; col++) {
      if (IsBackgroundBlock(row, col, m_pixels)) {
        m_bgBuffer[k] = 0;
      } else {
        m_bgBuffer[k] = 1;
      }
      k++;
    }
  }



  // now we scan the background frame, first horizontally and then vertically, 
  // to set the quantization levels

  // set the header valu so that the quant inthe first MB is always 0
  int highest = LOW_Q - STEP*(m_numLevels-1);

  if (m_bgBuffer[0]) {
    m_currentFrameHeaderQuant = highest;
  } else if (m_bgBuffer[1]) {
    m_currentFrameHeaderQuant = highest + STEP;
  } else {
    m_currentFrameHeaderQuant = LOW_Q;
  }
    
  // the horizontal scan
  int current_q = m_currentFrameHeaderQuant;

  
  int bl = 0;
  for (bl = 0 ; bl < m_horzBlocks*m_vertBlocks-2 ; bl++) 
  {
    m_quantBuffer[bl] = quants[(current_q-highest)/STEP][m_bgBuffer[bl]][m_bgBuffer[bl+1]][m_bgBuffer[bl+2]];
    current_q = m_quantBuffer[bl];
  }

  // handle 2 last blocks manually
  m_quantBuffer[bl] = m_bgBuffer[bl] ? highest : LOW_Q; bl++;
  m_quantBuffer[bl] = m_bgBuffer[bl] ? highest : LOW_Q; bl++;

  // the vertical scan
  current_q = m_currentFrameHeaderQuant;
  for (bl = 0 ; bl < m_horzBlocks*m_vertBlocks-2 ; bl++) 
  {
    int n0 = bl/m_vertBlocks + (bl%m_vertBlocks)*m_horzBlocks;
    if (m_quantBuffer[n0] == LOW_Q) {
      int n1 = (bl+1)/m_vertBlocks + ((bl+1)%m_vertBlocks)*m_horzBlocks;
      int n2 = (bl+2)/m_vertBlocks + ((bl+2)%m_vertBlocks)*m_horzBlocks;
      m_quantBuffer[n0] = quants[(current_q-highest)/STEP][m_bgBuffer[n0]][m_bgBuffer[n1]][m_bgBuffer[n2]];
    }
    current_q = m_quantBuffer[n0];
  }



  // calculate the differences for the hint buffer
  m_hintBuffer[0] = m_quantBuffer[0] - m_currentFrameHeaderQuant; 
  for (bl = 1 ; bl < m_horzBlocks*m_vertBlocks ; bl++) 
  {
    m_hintBuffer[bl] = m_quantBuffer[bl]-m_quantBuffer[bl-1];
  }
  
  return true;
}

/** Returns true if at least one pixel int he block is a foreground pixel */
Boolean Movie_recorder::IsBackgroundBlock(int row, int col, UChar *pixels) const
{
  int col_offset = col*BLOCK_SIZE;
  int row_offset = row*BLOCK_SIZE*m_width;
  int offset = 0;
  for (int r = 0 ; r < BLOCK_SIZE ; r++) {
    for (int c = 0 ; c < BLOCK_SIZE ; c++) {
      offset = r*m_width;
      if (pixels[col_offset+c + row_offset + offset] != 0) {
        return false;
      }
    }
  }
  return true;
}


void Movie_recorder::WriteBuffers() const
{
  // -----   write the rgb frame   ----------

  ELocalStream rgbStream(m_movieFullName);
  rgbStream.Open(EStream::EOpenMode::omAppend,  EStream::EOpenType::otBinary);
  if (m_totalFrames == 1) {
    // write an image header in the movie file
    char buffer[256];
    ULong written = 0;
    sprintf(buffer, "P6\n%d %d 255 ", m_width, m_height);
    rgbStream.BlockWrite(buffer, strlen(buffer), written);
  }
  int row, col;
  for (row = 0 ; row < m_height ; row++) {
    for (col = 0 ; col < m_width ; col++) {
      rgbStream << m_rgbBuffer[row*m_width*3+3*col] 
        << m_rgbBuffer[row*m_width*3+3*col+1]
        << m_rgbBuffer[row*m_width*3+3*col+2];
    }
  }
  rgbStream.Close();


  // ------  write hint frame    ----------

  String folder = EOperatingSystem::get_instance()->get_snapshotsFolder();
  ELocalStream test_stream(folder + "/" + m_hintFileName + "_quant.pgm");
  ELocalStream bg_stream(folder + "/" + m_hintFileName + "_bg.pgm");
  ELocalStream hintStream(m_hintFullName);
  hintStream.Open(EStream::EOpenMode::omAppend, EStream::EOpenType::otBinary);

  // if only one frame is created we write an image header in the files
  if (m_totalFrames == 1) {
    char buffer[256];
    sprintf(buffer, "P5\n%d %d 255 ", m_horzBlocks, m_vertBlocks);

    ULong written = 0;
    test_stream.Open(EStream::EOpenMode::omAppend, EStream::EOpenType::otBinary);
    test_stream.BlockWrite(buffer, strlen(buffer), written);

    bg_stream.Open(EStream::EOpenMode::omAppend, EStream::EOpenType::otBinary);
    bg_stream.BlockWrite(buffer, strlen(buffer), written);

    hintStream.BlockWrite(buffer, strlen(buffer), written);
  }

  int c = m_currentFrameHeaderQuant;
  hintStream << (char)m_currentFrameHeaderQuant;
  for (int bl = 0 ; bl < m_horzBlocks*m_vertBlocks ; bl++) 
  {
    hintStream << (char)m_hintBuffer[bl];

    if (m_totalFrames == 1) {
      c += m_hintBuffer[bl];
      test_stream << (char)(256/m_numLevels*(m_numLevels-c/STEP+1)-1);
      bg_stream << (char)(m_bgBuffer[bl]*255);
    }
  }
  hintStream.Close();

  if (m_totalFrames == 1) {
    test_stream.Close();
    bg_stream.Close();
  }
}
/**
 * Insert the image in m_snapPixels into the local stream "Stream"
 * concerning with ppm format file.
 */
void Movie_recorder::init_prototype()
{
  if (s_prototype!=NULL)
    return;

  // Allocate a prototype instance
  s_prototype = new Container_proto();

  s_prototype->add_field_info(new ESFBool(ENABLED,
                      "enabled",
                      get_member_offset(&m_isEnabled),
                      (Execution_func_type)&Movie_recorder::set_enabled));

  s_prototype->add_field_info(new ESFString(STATUS,
                      "status",
                      get_member_offset(&m_status)));
}

/*!
 */
void Movie_recorder::delete_prototype()
{
  delete s_prototype;
}

/*!
 */
Container_proto * Movie_recorder::get_prototype() 
{  
  if (!s_prototype) init_prototype();
  return s_prototype;
}

/*! Sets the attributes of the object extracted from the VRML or X3D file.
 * \param elem contains lists of attribute names and values
 * \param sg a pointer to the scene graph
 */
void Movie_recorder::set_attributes(Element * elem)
{
  Node::set_attributes(elem);
  for (Str_attr_iter ai = elem->str_attrs_begin();
       ai != elem->str_attrs_end(); ai++)
  {
    const std::string & name = elem->get_name(ai);
    const std::string & value = elem->get_value(ai);
    if (name == "movieFileName") {
      set_movieFileName(value);
      elem->mark_delete(ai);
    } else if (name == "hintFileName") {
      SetHintFileName(value);
      elem->mark_delete(ai);
    } else if (name == "totalFrames") { 
      SetTotalFrames(atoi(value.c_str()));
      elem->mark_delete(ai);
    } else if (name == "mode") { 
      SetRecordingMode(value);
      elem->mark_delete(ai);
    } else if (name == "transmitterIP") { 
      m_transmitterIP = value;
/*//      strcpy(m_transmitterIP, value.c_str());
      hostent *pHostent = gethostbyname("meiron.enbaya.org");
      String sttrIP = "10.0.0.137";
      //hostent *pHostent1 = gethostbyaddr(sttrIP.c_str(),sttrIP.length(),AF_INET);
      u_long res;
      hostent *pHostent1 ;

      if ((res = inet_addr ("10.0.0.137")) != INADDR_NONE)
      {
        pHostent1 = gethostbyaddr ((char*) &res, sizeof(res),AF_INET);
      }*/
      elem->mark_delete(ai);
    } else if (name == "transmitterPort") { 
      m_transmitterPort = atoi(value.c_str());
      elem->mark_delete(ai);
    } else if (name == "receiverIP") { 
      m_receiverIP = value;
//      strcpy(m_receiverIP, value.c_str());

      elem->mark_delete(ai);
    } else if (name == "receiverPort") { 
      m_receiverPort = atoi(value.c_str());
      elem->mark_delete(ai);
    } else if (name == "cif") { 
      StringVector aParametrs = std::stringUtils::SplitStringByToken(value, ",");
      assert(aParametrs.size() == 2);
      m_width = atoi(aParametrs[0].c_str());
      m_height = atoi(aParametrs[1].c_str());

      m_horzBlocks = m_width/BLOCK_SIZE;
      m_vertBlocks = m_height/BLOCK_SIZE;
      elem->mark_delete(ai);
    } else if (name == "hintType") {
      m_hintType = value;
      elem->mark_delete(ai);
    }
  }
  // Remove all the marked attributes:
  elem->delete_marked();
}

/*!
 */
Attribute_list Movie_recorder::get_attributes()
{
  Attribute_list attribs; 
  Attribue attrib;
  char buf[32];

  attribs = Node::get_attributes();

  if (m_movieFileName != m_defMovieFileName) 
  {
    attrib.first = "movieFileName";
    attrib.second = get_movieFileName();
    attribs.push_back(attrib);
  }

  if (m_hintFileName != m_defHintFileName) 
  {
    attrib.first = "hintFileName";
    attrib.second = GetHintFileName();
    attribs.push_back(attrib);
  }

  if (m_totalFrames != m_defTotalFrames) 
  {
    attrib.first = "totalFrames";
    sprintf(buf, "%d", m_totalFrames);
    attrib.second = buf;
    attribs.push_back(attrib);
  }

  if (m_mode != m_defMode) 
  {
    attrib.first = "mode";
    attrib.second = "socket";
    attribs.push_back(attrib);
  }

  if (m_mode == rmSocket) 
  {
    attrib.first = "ip";
    attrib.second = m_transmitterIP;
    attribs.push_back(attrib);

    attrib.first = "port";
    sprintf(buf, "%d", m_transmitterPort);
    attrib.second = buf;
    attribs.push_back(attrib);
  }

  return attribs; 
}

void Movie_recorder::AddToScene(Scene_graph *sg, XML_entity *parent)
{
  Node::AddToScene(sg, parent);
  m_context = sg->get_context();
  m_sceneGraph = sg;
}


/* --------  documentation  ----------------------------

  The movie file contains n frames of RGB triplets, written in binary format. 
  The hint file contains a single byte signed value ranges from -4 to 4. Each 
  value is the difference in the quantization level ob the current block relative 
  to the previous block. The changes in the levels are usually in steps of 2 
  (using the STEP variable). We currently support 2 formats. 3 level quantization and 
  4 (the default) level quantization. The highest quantization level (lowest quality)
  is specified with the LOW_Q variable. The level of quantization is set according to the 
  following tables. The coumns are as following: 

       The     | The next | The second | next   | difference
     current   |  block   | next block | block  | (stored in 
   block quant | (F, B)   |   (F, B)   | quant. | the hint file) 

  where: F - Foregroung, B - Background.

    In case of 3 levels:
    4 B B 6 2
      B F 6 2
      F B 4 0
      F F 4 0
    6 B B 8 2
      B F 6 0
      F B 4 -2
      F F 4 -2
    8 B B 8 0
      B F 6 -2
      F B 4 -4
      F F 6 -2

    In case of 4 levels:
    2 B B 4 2 
      B F 4 2
      F B 4 2
      F F 2 0
    4 B B 6 2
      B F 4 0
      F B 4 0
      F F 2 -2
    6 B B 8 2
      B F 6 0
      F B 4 -2
      F F 4 -2
    8 B B 8 0
      B F 6 -2
      F B 4 -4
      F F 4 -4



*/
  /*
  static int table4[][2][2] = {
    {{2, 2}, {2, 0}  },
    {{2, 0}, {0, -2}},
    {{2, 0}, {-2, -2}},
    {{0, -2},{-4, -4}}};
    */
unsigned Movie_recorder::ThreadHandlerProc(void)
{
  if (!Init(m_transmitterIP, m_transmitterPort))
    MessageBox(NULL,"Error in initializing socket","ERemoteController",MB_OK);
//  else
//    MessageBox(NULL,"socket init OK","ERemoteController",MB_OK);

//  String installationFolder = 
//  EOperatingSystem::get_instance()->get_installationFolder();
//  installationFolder += "\\RunTransmitter.bat";

//  ShellExecute(NULL, "open", installationFolder.c_str(), NULL, NULL, SW_SHOWNORMAL);
//  String machineIP = get_machineIPStr();
  String param;
  param.format(" VideoTransmit socket:%s %s %d",m_transmitterIP,m_receiverIP,m_receiverPort);
  ShellExecute(NULL, "open", "java", param.c_str(), NULL, SW_SHOWNORMAL);
  StartMonitoring();

  return THREAD_NORMAL_EXIT_CODE;
}

void Movie_recorder::Start()
{
  m_pThread = new TThread(this);
  ASSERT(m_pThread);
}

void Movie_recorder::OnAccept()
{
  char data[256];
  sprintf(data, "%4d|%4d|%6d", m_width, m_height, 0);
  Reply((char*)data, strlen(data));
}

void Movie_recorder::OnData(char *inBuff,int len)
{
}

void Movie_recorder::CheckKill()
{
}

void RunJavaThread()
{
//  Sleep(1000);
  //ShellExecute();
  String installationFolder = 
    EOperatingSystem::get_instance()->get_installationFolder();

  ShellExecute(NULL, "open", "C:\\Documents and Settings\\meiron\\Desktop\\tst.bat", NULL, NULL, SW_MINIMIZE );//SW_SHOWNORMAL);
}

SGAL_END_NAMESPACE
