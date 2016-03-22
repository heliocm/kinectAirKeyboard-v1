/*****************************************************************************
*                                                                            *
*  OpenNI 1.x Alpha                                                          *
*  Copyright (C) 2012 PrimeSense Ltd.                                        *
*                                                                            *
*  This file is part of OpenNI.                                              *
*                                                                            *
*  Licensed under the Apache License, Version 2.0 (the "License");           *
*  you may not use this file except in compliance with the License.          *
*  You may obtain a copy of the License at                                   *
*                                                                            *
*      http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                            *
*  Unless required by applicable law or agreed to in writing, software       *
*  distributed under the License is distributed on an "AS IS" BASIS,         *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
*  See the License for the specific language governing permissions and       *
*  limitations under the License.                                            *
*                                                                            *
*****************************************************************************/
//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include <thread>
#include "NiHandViewer.h"
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#if (XN_PLATFORM == XN_PLATFORM_MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

//---------------------------------------------------------------------------
// Defines
//---------------------------------------------------------------------------
#define LENGTHOF(arr)			(sizeof(arr)/sizeof(arr[0]))
#define GL_WIN_SIZE_X	1280
#define GL_WIN_SIZE_Y	1024

//---------------------------------------------------------------------------
// Method Definitions
//---------------------------------------------------------------------------
float*handX, *handY;
bool inside = false;
SimpleViewer& HandViewer::CreateInstance( xn::Context& context )
{
	assert(!sm_pInstance);
	return *(sm_pInstance = new HandViewer(context));
}

HandViewer::HandViewer(xn::Context& context)
:SimpleViewer(context),
m_HandTracker(context)
{}

XnStatus HandViewer::Init(int argc, char **argv)
{
	XnStatus rc;
	rc = SimpleViewer::Init(argc, argv);
	if(rc != XN_STATUS_OK)
	{
		return rc;
	}

	return m_HandTracker.Init();
}

XnStatus HandViewer::Run()
{
	XnStatus rc = m_HandTracker.Run();

	if(rc != XN_STATUS_OK)
	{
		return rc;
	}

	return SimpleViewer::Run(); // Does not return, enters OpenGL main loop instead
}

void playNote (int file){
	
	int out;
	if (file == 0)
		out = system("aplay ~/Documents/C1.wav");
	else if (file == 1)
		out = system("aplay ~/Documents/C#.wav");
	else if (file == 2)
		out = system("aplay ~/Documents/D.wav");
	else if (file == 3)
		out = system("aplay ~/Documents/D#.wav");
	else if (file == 4)
		out = system("aplay ~/Documents/E.wav");
	else if (file == 5)
		out = system("aplay ~/Documents/F.wav");
	else if (file == 6)
		out = system("aplay ~/Documents/F#.wav");
	else if (file == 7)
		out = system("aplay ~/Documents/G.wav");
	else if (file == 8)
		out = system("aplay ~/Documents/G#.wav");
	else if (file == 9)
		out = system("aplay ~/Documents/A.wav");
	else if (file == 10)
		out = system("aplay ~/Documents/A#.wav");
	else if (file == 11)
		out = system("aplay ~/Documents/B.wav");
	else if (file == 12)
		out = system("aplay ~/Documents/C2.wav");
}

void HandViewer::DisplayPostDraw()
{
	typedef TrailHistory			History;
	typedef History::ConstIterator	HistoryIterator;
	typedef Trail::ConstIterator	TrailIterator;

	static const float colours[][3] =
	{
		{ 0.5f, 0.5f, 0.5f},
		{ 0.0f, 1.0f, 0.0f},
		{ 0.0f, 0.5f, 1.0f},
		{ 1.0f, 1.0f, 0.0f},
		{ 1.0f, 0.5f, 0.0f},
		{ 1.0f, 0.0f, 1.0f}
	};
	const TrailHistory&	history = m_HandTracker.GetHistory();

	// History points coordinates buffer
	XnFloat	coordinates[3 * MAX_HAND_TRAIL_LENGTH];

	const HistoryIterator	hend = history.End();
	for(HistoryIterator		hit = history.Begin(); hit != hend; ++hit)
	{

		// Dump the history to local buffer
		int				numpoints = 0;
		const Trail&	trail = hit->Value();

		const TrailIterator	tend = trail.End();
		for(TrailIterator	tit = trail.Begin(); tit != tend; ++tit)
		{
			XnPoint3D	point = *tit;
			m_depth.ConvertRealWorldToProjective(1, &point, &point);
			ScalePoint(point);
			coordinates[numpoints * 3] = point.X;
			coordinates[numpoints * 3 + 1] = point.Y;
			coordinates[numpoints * 3 + 2] = 0;
			std::cout << point.X << ',' << point.Y << std::endl;
			handX = &point.X;
			handY = &point.Y;
			++numpoints;
		}
		assert(numpoints <= MAX_HAND_TRAIL_LENGTH);

		// Draw the hand trail history
		XnUInt32 nColor = hit->Key() % LENGTHOF(colours);
		glColor4f(colours[nColor][0],
			colours[nColor][1],
			colours[nColor][2],
			1.0f);
		glPointSize(2);
		glVertexPointer(3, GL_FLOAT, 0, coordinates);
		//glDrawArrays(GL_LINE_STRIP, 0, numpoints);
		// Current point as a larger dot
		glPointSize(50);
		glDrawArrays(GL_POINTS, 0, 1);
		glColor4f(1,0,0,1);
		for(int i = 1; i <= 13; i++){
			glBegin(GL_LINE_LOOP);
		    glVertex2f( 3 + (i-1)*GL_WIN_SIZE_X/13, GL_WIN_SIZE_Y - 200 );
		    glVertex2f( 3 + (i-1)*GL_WIN_SIZE_X/13, GL_WIN_SIZE_Y );
		    glVertex2f( 3 + i*GL_WIN_SIZE_X/13, GL_WIN_SIZE_Y );
		    glVertex2f( 3 + i*GL_WIN_SIZE_X/13, GL_WIN_SIZE_Y - 200 );
		    glEnd();
		}
		// Play Notes
		//C1
		if((*handX<GL_WIN_SIZE_X/13)&&(*handY>GL_WIN_SIZE_Y-200)&&(inside==false)){ 
				//int out = system("aplay ~/Documents/C1.wav");
				std::thread music(playNote,0);
				music.detach();
				inside = true;
			}
		//C#
		else if((*handX>GL_WIN_SIZE_X/13)&&(*handX<2*GL_WIN_SIZE_X/13)&&(*handY>GL_WIN_SIZE_Y-200)&&(inside==false)){
				//int out = system("aplay ~/Documents/C#.wav");
				std::thread music(playNote,1);
				music.detach();
				inside = true;
			}
		//D
		else if((*handX>2*GL_WIN_SIZE_X/13)&&(*handX<3*GL_WIN_SIZE_X/13)&&(*handY>GL_WIN_SIZE_Y-200)&&(inside==false)){
				//int out = system("aplay ~/Documents/D.wav");
				std::thread music(playNote,2);
				music.detach();
				inside = true;
			}
		//D#
		else if((*handX>3*GL_WIN_SIZE_X/13)&&(*handX<4*GL_WIN_SIZE_X/13)&&(*handY>GL_WIN_SIZE_Y-200)&&(inside==false)){
				//int out = system("aplay ~/Documents/D#.wav");
				std::thread music(playNote,3);
				music.detach();
				inside = true;
			}
		//E
		else if((*handX>4*GL_WIN_SIZE_X/13)&&(*handX<5*GL_WIN_SIZE_X/13)&&(*handY>GL_WIN_SIZE_Y-200)&&(inside==false)){
				//int out = system("aplay ~/Documents/E.wav");
				std::thread music(playNote,4);
				music.detach();
				inside = true;
			}
		//F
		else if((*handX>5*GL_WIN_SIZE_X/13)&&(*handX<6*GL_WIN_SIZE_X/13)&&(*handY>GL_WIN_SIZE_Y-200)&&(inside==false)){
				//int out = system("aplay ~/Documents/F.wav");
				std::thread music(playNote,5);
				music.detach();
				inside = true;
			}
		//F#
		else if((*handX>6*GL_WIN_SIZE_X/13)&&(*handX<7*GL_WIN_SIZE_X/13)&&(*handY>GL_WIN_SIZE_Y-200)&&(inside==false)){
				//int out = system("aplay ~/Documents/F#.wav");
				std::thread music(playNote,6);
				music.detach();
				inside = true;
			}
		//G
		else if((*handX>7*GL_WIN_SIZE_X/13)&&(*handX<8*GL_WIN_SIZE_X/13)&&(*handY>GL_WIN_SIZE_Y-200)&&(inside==false)){
				//int out = system("aplay ~/Documents/G.wav");
				std::thread music(playNote,7);
				music.detach();
				inside = true;
			}
		//G#
		else if((*handX>8*GL_WIN_SIZE_X/13)&&(*handX<9*GL_WIN_SIZE_X/13)&&(*handY>GL_WIN_SIZE_Y-200)&&(inside==false)){
				//int out = system("aplay ~/Documents/G#.wav");
				std::thread music(playNote,8);
				music.detach();
				inside = true;
			}
		//A
		else if((*handX>9*GL_WIN_SIZE_X/13)&&(*handX<10*GL_WIN_SIZE_X/13)&&(*handY>GL_WIN_SIZE_Y-200)&&(inside==false)){
				//int out = system("aplay ~/Documents/A.wav");
				std::thread music(playNote,9);
				music.detach();
				inside = true;
			}
		//A#
		else if((*handX>10*GL_WIN_SIZE_X/13)&&(*handX<11*GL_WIN_SIZE_X/13)&&(*handY>GL_WIN_SIZE_Y-200)&&(inside==false)){
				//int out = system("aplay ~/Documents/A#.wav");
				std::thread music(playNote,10);
				music.detach();
				inside = true;
			}
		//B
		else if((*handX>11*GL_WIN_SIZE_X/13)&&(*handX<12*GL_WIN_SIZE_X/13)&&(*handY>GL_WIN_SIZE_Y-200)&&(inside==false)){
				//int out = system("aplay ~/Documents/B.wav");
				std::thread music(playNote,11);
				music.detach();
				inside = true;
			}
		//C2
		else if((*handX>12*GL_WIN_SIZE_X/13)&&(*handX<13*GL_WIN_SIZE_X/13)&&(*handY>GL_WIN_SIZE_Y-200)&&(inside==false)){
				//int out = system("aplay ~/Documents/C2.wav");
				std::thread music(playNote,12);
				music.detach();
				inside = true;
			}
		else if((*handY<GL_WIN_SIZE_Y-200)&&(inside==true)){
				inside = false;
			}	
		glFlush();
	}
}


XnStatus HandViewer::InitOpenGL( int argc, char **argv )
{
	XnStatus rc = SimpleViewer::InitOpenGL(argc, argv); 

	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	return rc;
}
