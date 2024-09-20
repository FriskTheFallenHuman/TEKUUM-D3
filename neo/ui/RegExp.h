/*
===========================================================================

KROOM 3 GPL Source Code

This file is part of the KROOM 3 Source Code, originally based
on the Doom 3 with bits and pieces from Doom 3 BFG edition GPL Source Codes both published in 2011 and 2012.

KROOM 3 Source Code is free software: you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation, either version 3 of the License,
or (at your option) any later version. For details, see LICENSE.TXT.

Extra attributions can be found on the CREDITS.txt file

===========================================================================
*/

#ifndef __REGEXP_H__
#define __REGEXP_H__

class idTokenParser;
class idWindow;
class idWinVar;

class idRegister
{
public:
	idRegister();
	idRegister( const char* p, int t );

	enum REGTYPE { VEC4 = 0, FLOAT, BOOL, INT, STRING, VEC2, VEC3, RECTANGLE, NUMTYPES } ;
	static int REGCOUNT[NUMTYPES];

	bool				enabled;
	short				type;
	idStr				name;
	int					regCount;
	unsigned short		regs[4];
	idWinVar* 			var;

	void				SetToRegs( float* registers );
	void				GetFromRegs( float* registers );
	void				CopyRegs( idRegister* src );
	void				Enable( bool b )
	{
		enabled = b;
	}
	void				ReadFromDemoFile( idDemoFile* f );
	void				WriteToDemoFile( idDemoFile* f );
	void				WriteToSaveGame( idFile* savefile );
	void				ReadFromSaveGame( idFile* savefile );
};

ID_INLINE idRegister::idRegister()
{
}

ID_INLINE idRegister::idRegister( const char* p, int t )
{
	name = p;
	type = t;
	assert( t >= 0 && t < NUMTYPES );
	regCount = REGCOUNT[t];
	enabled = ( type == STRING ) ? false : true;
	var = NULL;
};

ID_INLINE void idRegister::CopyRegs( idRegister* src )
{
	regs[0] = src->regs[0];
	regs[1] = src->regs[1];
	regs[2] = src->regs[2];
	regs[3] = src->regs[3];
}

class idRegisterList
{
public:

	idRegisterList();
	~idRegisterList();

	void				AddReg( const char* name, int type, idTokenParser* src, idWindow* win, idWinVar* var );
	void				AddReg( const char* name, int type, idParser* src, idWindow* win, idWinVar* var );
	void				AddReg( const char* name, int type, idVec4 data, idWindow* win, idWinVar* var );

	idRegister* 		FindReg( const char* name );
	void				SetToRegs( float* registers );
	void				GetFromRegs( float* registers );
	void				Reset();
	void				ReadFromDemoFile( idDemoFile* f );
	void				WriteToDemoFile( idDemoFile* f );
	void				WriteToSaveGame( idFile* savefile );
	void				ReadFromSaveGame( idFile* savefile );

private:
	idList<idRegister*>	regs;
	idHashIndex			regHash;
};

ID_INLINE idRegisterList::idRegisterList()
{
	regs.SetGranularity( 4 );
	regHash.SetGranularity( 4 );
	regHash.Clear( 32, 4 );
}

ID_INLINE idRegisterList::~idRegisterList()
{
}

#endif /* !__REGEXP_H__ */
