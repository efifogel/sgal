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
// $Id: $
// $Revision: 6147 $
//
// Author(s)     : Efi Fogel         <efifogel@gmail.com>

/**
 * @class: EMovieRecorder
 *
 * Purpose: To record a movie sequence.
 *
 * Description:  This is used to produce a movie file for the MPEG-4 project.
 *
 * Inherits from EEvent_filter, Node, TThreadHandler, ESocketUtil
 *
 * authors: Tali zvi, Meiron Cohen
 */

#ifndef SGAL_MOVIE_RECORDER_HPP
#define SGAL_MOVIE_RECORDER_HPP

#include "SGAL/basic.hpp"
#include "SGAL/Node.h"
#include "SGAL/Event_filter.h"
#include "SGAL/Context.h"
#include "SGAL/Event.h"

#include "SGAL/Thread.h"
#include "SGAL/SocketUtil.h"

SGAL_BEGIN_NAMESPACE

class Element;
class Container_proto;
class Scene_graph;

enum Movie_recorder { rmFile, rmSocket };

class SGAL_SGAL_DECL Movie_recorder :
  public EEvent_filter,
  public Node,
  public TThreadHandler,
  public ESocketUtil
{
public:
  enum {
    FIRST = Container::LAST-1,
    ENABLED,
    STATUS,
    LAST
  };

  /*! Constructor. */
  Movie_recorder(Boolean proto = false);

  /*! Destructor. */
  virtual ~Movie_recorder();

  /* Construct the prototype. */
  static Movie_recorder* prototype();

  /*! Clone. */
  virtual Container* clone();

 /*! Initialize the container prototype. */
  virtual void init_prototype();
  virtual void delete_prototype();
  virtual Container_proto *get_prototype();

  /*! Sets the attributes of this node */
  virtual void set_attributes(Element* elem);

  virtual Attribute_list get_attributes();

  void set_enabled( Field_info* field_info = NULL );
  virtual Action::Trav_directive Draw(Draw_action* draw_action)
  { return Action::Trav_cont; };

  virtual bool UpdateTime  ();

  void set_movieFileName( String filename );

  String get_movieFileName() const { return m_movieFileName; }

  void SetHintFileName( String filename );

  String GetHintFileName() const { return m_hintFileName; }

  void Movie_recorder::SetTotalFrames(int num);

  int get_totalFrames() const { return m_totalFrames; }

  void SetRecordingMode(const String mode);

  Movie_recorder GetRecordingMode() const { return m_mode; }

  unsigned ThreadHandlerProc(void);

  void Start();

  void OnData(char* inBuff = NULL,int len = 0);

  void OnAccept();

protected:
  void CheckKill();

private:
  static Container_proto* s_prototype;

  /*! the name of the movie file */
  String m_movieFileName;
  String m_movieFullName;

  /*! the name of the hint file */
  String m_hintFileName;
  String m_hintFullName;

  /*! The trigger */
  Boolean m_isEnabled;

  /*! The number of layers used */
  Short m_numLevels;

  /*! The current number of frames */
  Int m_frameNo;

  /*! The total number of frames to store */
  Int m_totalFrames;

  /*! A pointer to the context. Used to get the viewpoint */
  Context* m_context;

  /*! A pointer to the scene graph */
  Scene_graph* m_sceneGraph;

  /*! The mode of recording (file or socket) */
  Movie_recorder m_mode;

  /*! The ip to connect to */
  String m_transmitterIP;
  String m_receiverIP;

  /*! The port to connect to */
  UShort m_transmitterPort;
  UShort m_receiverPort;

  /*! The status of the movie */
  std::string m_status;

  TEvent m_KillEvent;
  UChar* m_pixels;
  UChar* m_rgbBuffer;
  UChar* m_rgbBuffer2;
  UChar* m_hintBuffer;
  int m_currentFrameHeaderQuant;

  short* m_bgBuffer;
  short* m_quantBuffer;

  /*! Frame width */
  int m_width;

  /*! Frame height */
  int m_height;

  /*! Number of horizontal blocks */
  long m_horzBlocks;

  /*! Number of vertical blocks */
  long m_vertBlocks;

  TThread* m_pThread;

  String m_hintType;

  // default values for fields
  const static char* m_defMovieFileName;
  const static char* m_defHintFileName;
  const static Int m_defTotalFrames;
  const static Short m_defNumLevels;
  const static int m_defWidth;
  const static int m_defHeight;
  const static Movie_recorder m_defMode;

  Boolean IsBackgroundBlock(int row, int col, UChar* pixels) const;
  void WriteBuffers() const;
  bool ProcessFrame();
};

/* \brief constructs the prototype. */
inline Movie_recorder* Movie_recorder::prototype()
{ return new Movie_recorder(true); }

/*! \brief clones. */
inline Container* Movie_recorder::clone() { return new Movie_recorder(); }

void RunJavaThread();

SGAL_END_NAMESPACE

#endif
