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

#ifndef __MODEL_LOCAL_H__
#define __MODEL_LOCAL_H__

/*
===============================================================================

	Static model

===============================================================================
*/

class idJointMat;
struct deformInfo_t;
struct objModel_t;		// RB: Wavefront OBJ support

class idRenderModelStatic : public idRenderModel
{
public:
	// the inherited public interface
	static idRenderModel* 		Alloc();

	idRenderModelStatic();
	virtual						~idRenderModelStatic();

	virtual void				InitFromFile( const char* fileName );
	virtual bool				LoadBinaryModel( idFile* file, const ID_TIME_T sourceTimeStamp );
	virtual void				WriteBinaryModel( idFile* file, ID_TIME_T* _timeStamp = NULL ) const;
	virtual bool				SupportsBinaryModel()
	{
		return true;
	}

	virtual void				PartialInitFromFile( const char* fileName );
	virtual void				PurgeModel();
	virtual void				Reset() {};
	virtual void				LoadModel();
	virtual bool				IsLoaded();
	virtual void				SetLevelLoadReferenced( bool referenced );
	virtual bool				IsLevelLoadReferenced();
	virtual void				TouchData();
	virtual void				InitEmpty( const char* name );
	virtual void				AddSurface( modelSurface_t surface );
	virtual void				FinishSurfaces( bool useMikktspace );
	virtual void				CreateVertexCache();
	virtual void				FreeVertexCache();
	virtual const char* 		Name() const;
	virtual void				Print() const;
	virtual void				List() const;
	virtual int					Memory() const;
	virtual ID_TIME_T			Timestamp() const;
	virtual int					NumSurfaces() const;
	virtual int					NumBaseSurfaces() const;
	virtual const modelSurface_t* Surface( int surfaceNum ) const;
	virtual srfTriangles_t* 	AllocSurfaceTriangles( int numVerts, int numIndexes ) const;
	virtual void				FreeSurfaceTriangles( srfTriangles_t* tris ) const;
	virtual bool				IsStaticWorldModel() const;
	virtual dynamicModel_t		IsDynamicModel() const;
	virtual bool				IsDefaultModel() const;
	virtual bool				IsReloadable() const;
	virtual idRenderModel* 		InstantiateDynamicModel( const struct renderEntity_s* ent, const viewDef_t* view, idRenderModel* cachedModel );
	virtual int					NumJoints() const;
	virtual const idMD5Joint* 	GetJoints() const;
	virtual jointHandle_t		GetJointHandle( const char* name ) const;
	virtual const char* 		GetJointName( jointHandle_t handle ) const;
	virtual const idJointQuat* 	GetDefaultPose() const;
	virtual int					NearestJoint( int surfaceNum, int a, int b, int c ) const;
	virtual idBounds			Bounds( const struct renderEntity_s* ent ) const;
	virtual void				ReadFromDemoFile( class idDemoFile* f );
	virtual void				WriteToDemoFile( class idDemoFile* f );
	virtual float				DepthHack() const;

	virtual bool				ModelHasDrawingSurfaces() const
	{
		return hasDrawingSurfaces;
	};
	virtual bool				ModelHasInteractingSurfaces() const
	{
		return hasInteractingSurfaces;
	};
	virtual bool				ModelHasShadowCastingSurfaces() const
	{
		return hasShadowCastingSurfaces;
	};

	void						MakeDefaultModel();

	bool						LoadASE( const char* fileName, ID_TIME_T* sourceTimeStamp );
	bool						LoadLWO( const char* fileName, ID_TIME_T* sourceTimeStamp );
	bool						LoadMA( const char* filename, ID_TIME_T* sourceTimeStamp );
	bool						LoadOBJ( const char* fileName, ID_TIME_T* sourceTimeStamp ); // RB

	bool						ConvertOBJToModelSurfaces( const objModel_t* obj ); // RB
	bool						ConvertASEToModelSurfaces( const struct aseModel_s* ase );
	bool						ConvertLWOToModelSurfaces( const struct st_lwObject* lwo );
	bool						ConvertMAToModelSurfaces( const struct maModel_s* ma );

	struct aseModel_s* 			ConvertLWOToASE( const struct st_lwObject* obj, const char* fileName );

	bool						DeleteSurfaceWithId( int id );
	void						DeleteSurfacesWithNegativeId();
	bool						FindSurfaceWithId( int id, int& surfaceNum ) const;

public:
	idList<modelSurface_t>	surfaces;
	idBounds					bounds;
	int							overlaysAdded;

	// when an md5 is instantiated, the inverted joints array is stored to allow GPU skinning
	int							numInvertedJoints;
	idJointMat* 				jointsInverted;
	vertCacheHandle_t			jointsInvertedBuffer;

protected:
	int							lastModifiedFrame;
	int							lastArchivedFrame;

	idStr						name;
	bool						isStaticWorldModel;
	bool						defaulted;
	bool						purged;					// eventually we will have dynamic reloading
	bool						fastLoad;				// don't generate tangents and shadow data
	bool						reloadable;				// if not, reloadModels won't check timestamp
	bool						levelLoadReferenced;	// for determining if it needs to be freed
	bool						hasDrawingSurfaces;
	bool						hasInteractingSurfaces;
	bool						hasShadowCastingSurfaces;
	ID_TIME_T					timeStamp;

	static idCVar				r_mergeModelSurfaces;	// combine model surfaces with the same material
	static idCVar				r_slopVertex;			// merge xyz coordinates this far apart
	static idCVar				r_slopTexCoord;			// merge texture coordinates this far apart
	static idCVar				r_slopNormal;			// merge normals that dot less than this
};

/*
===============================================================================

	MD5 animated model

===============================================================================
*/

class idMD5Mesh
{
	friend class				idRenderModelMD5;

public:
	idMD5Mesh();
	~idMD5Mesh();

	void						ParseMesh( idLexer& parser, int numJoints, const idJointMat* joints );

	int							NumVerts() const
	{
		return numVerts;
	}
	int							NumTris() const
	{
		return numTris;
	}

	void						UpdateSurface( const struct renderEntity_s* ent, const idJointMat* joints,
			const idJointMat* entJointsInverted, modelSurface_t* surf );
	void						CalculateBounds( const idJointMat* entJoints, idBounds& bounds ) const;
	int							NearestJoint( int a, int b, int c ) const;

private:
	const idMaterial* 			shader;				// material applied to mesh
	int							numVerts;			// number of vertices
	int							numTris;			// number of triangles
	byte* 						meshJoints;			// the joints used by this mesh
	int							numMeshJoints;		// number of mesh joints
	float						maxJointVertDist;	// maximum distance a vertex is separated from a joint
	deformInfo_t* 				deformInfo;			// used to create srfTriangles_t from base frames and new vertexes
	int							surfaceNum;			// number of the static surface created for this mesh
};

class idRenderModelMD5 : public idRenderModelStatic
{
public:
	virtual void				InitFromFile( const char* fileName );
	virtual bool				LoadBinaryModel( idFile* file, const ID_TIME_T sourceTimeStamp );
	virtual void				WriteBinaryModel( idFile* file, ID_TIME_T* _timeStamp = NULL ) const;
	virtual dynamicModel_t		IsDynamicModel() const;
	virtual idBounds			Bounds( const struct renderEntity_s* ent ) const;
	virtual void				Print() const;
	virtual void				List() const;
	virtual void				TouchData();
	virtual void				PurgeModel();
	virtual void				LoadModel();
	virtual int					Memory() const;
	virtual idRenderModel* 		InstantiateDynamicModel( const struct renderEntity_s* ent, const viewDef_t* view, idRenderModel* cachedModel );
	virtual int					NumJoints() const;
	virtual const idMD5Joint* 	GetJoints() const;
	virtual jointHandle_t		GetJointHandle( const char* name ) const;
	virtual const char* 		GetJointName( jointHandle_t handle ) const;
	virtual const idJointQuat* 	GetDefaultPose() const;
	virtual int					NearestJoint( int surfaceNum, int a, int b, int c ) const;

	virtual bool				SupportsBinaryModel()
	{
		return true;
	}

private:
	idList<idMD5Joint>	joints;
	idList<idJointQuat>	defaultPose;
	idList<idJointMat>	invertedDefaultPose;
	idList<idMD5Mesh>	meshes;

	void						DrawJoints( const renderEntity_t* ent, const viewDef_t* view ) const;
	void						ParseJoint( idLexer& parser, idMD5Joint* joint, idJointQuat* defaultPose );
};

/*
===============================================================================

	MD3 animated model

===============================================================================
*/

struct md3Header_s;
struct md3Surface_s;

class idRenderModelMD3 : public idRenderModelStatic
{
public:
	idRenderModelMD3();

	virtual void				InitFromFile( const char* fileName );
	virtual bool				SupportsBinaryModel()
	{
		return false;
	}
	virtual dynamicModel_t		IsDynamicModel() const;
	virtual idRenderModel* 		InstantiateDynamicModel( const struct renderEntity_s* ent, const viewDef_t* view, idRenderModel* cachedModel );
	virtual idBounds			Bounds( const struct renderEntity_s* ent ) const;

private:
	int							index;			// model = tr.models[model->index]
	int							dataSize;		// just for listing purposes
	struct md3Header_s* 		md3;			// only if type == MOD_MESH
	int							numLods;
	idList<const idMaterial*>	shaders;		// DG: md3Shader_t::shaderIndex indexes into this array

	void						LerpMeshVertexes( srfTriangles_t* tri, const struct md3Surface_s* surf, const float backlerp, const int frame, const int oldframe ) const;
};

/*
===============================================================================

	Liquid model

===============================================================================
*/

class idRenderModelLiquid : public idRenderModelStatic
{
public:
	idRenderModelLiquid();

	virtual void				InitFromFile( const char* fileName );
	virtual bool				SupportsBinaryModel()
	{
		return false;
	}
	virtual dynamicModel_t		IsDynamicModel() const;
	virtual idRenderModel* 		InstantiateDynamicModel( const struct renderEntity_s* ent, const viewDef_t* view, idRenderModel* cachedModel );
	virtual idBounds			Bounds( const struct renderEntity_s* ent ) const;

	virtual void				Reset();
	void						IntersectBounds( const idBounds& bounds, float displacement );

private:
	modelSurface_t				GenerateSurface( float lerp );
	void						WaterDrop( int x, int y, float* page );
	void						Update();

	int							verts_x;
	int							verts_y;
	float						scale_x;
	float						scale_y;
	int							time;
	int							liquid_type;
	int							update_tics;
	int							seed;

	idRandom					random;

	const idMaterial* 			shader;
	deformInfo_t* 				deformInfo;		// used to create srfTriangles_t from base frames
	// and new vertexes

	float						density;
	float						drop_height;
	int							drop_radius;
	float						drop_delay;

	idList<float>	pages;
	float* 						page1;
	float* 						page2;

	idList<idDrawVert>	verts;

	int							nextDropTime;

};

/*
===============================================================================

	PRT model

===============================================================================
*/

class idRenderModelPrt : public idRenderModelStatic
{
public:
	idRenderModelPrt();

	virtual void				InitFromFile( const char* fileName );
	virtual bool				SupportsBinaryModel()
	{
		return false;
	}
	virtual void				TouchData();
	virtual dynamicModel_t		IsDynamicModel() const;
	virtual idRenderModel* 		InstantiateDynamicModel( const struct renderEntity_s* ent, const viewDef_t* view, idRenderModel* cachedModel );
	virtual idBounds			Bounds( const struct renderEntity_s* ent ) const;
	virtual float				DepthHack() const;
	virtual int					Memory() const;

	// with the addModels2 arrangement we could have light accepting and
	// shadowing dynamic models, but the original game never did
	virtual bool				ModelHasDrawingSurfaces() const
	{
		return true;
	};
	virtual bool				ModelHasInteractingSurfaces() const
	{
		return false;
	};
	virtual bool				ModelHasShadowCastingSurfaces() const
	{
		return false;
	};

private:
	const idDeclParticle* 		particleSystem;
};

/*
===============================================================================

	Beam model

===============================================================================
*/

class idRenderModelBeam : public idRenderModelStatic
{
public:
	virtual dynamicModel_t		IsDynamicModel() const;
	virtual bool				SupportsBinaryModel()
	{
		return false;
	}
	virtual bool				IsLoaded() const;
	virtual idRenderModel* 		InstantiateDynamicModel( const struct renderEntity_s* ent, const viewDef_t* view, idRenderModel* cachedModel );
	virtual idBounds			Bounds( const struct renderEntity_s* ent ) const;

	// with the addModels2 arrangement we could have light accepting and
	// shadowing dynamic models, but the original game never did
	virtual bool				ModelHasDrawingSurfaces() const
	{
		return true;
	};
	virtual bool				ModelHasInteractingSurfaces() const
	{
		return false;
	};
	virtual bool				ModelHasShadowCastingSurfaces() const
	{
		return false;
	};
};

/*
===============================================================================

	Beam model

===============================================================================
*/
#define MAX_TRAIL_PTS	20

struct Trail_t
{
	int							lastUpdateTime;
	int							duration;

	idVec3						pts[MAX_TRAIL_PTS];
	int							numPoints;
};

class idRenderModelTrail : public idRenderModelStatic
{
	idList<Trail_t>	trails;
	int							numActive;
	idBounds					trailBounds;

public:
	idRenderModelTrail();

	virtual dynamicModel_t		IsDynamicModel() const;
	virtual bool				SupportsBinaryModel()
	{
		return false;
	}
	virtual bool				IsLoaded() const;
	virtual idRenderModel* 		InstantiateDynamicModel( const struct renderEntity_s* ent, const viewDef_t* view, idRenderModel* cachedModel );
	virtual idBounds			Bounds( const struct renderEntity_s* ent ) const;

	// with the addModels2 arrangement we could have light accepting and
	// shadowing dynamic models, but the original game never did
	virtual bool				ModelHasDrawingSurfaces() const
	{
		return true;
	};
	virtual bool				ModelHasInteractingSurfaces() const
	{
		return false;
	};
	virtual bool				ModelHasShadowCastingSurfaces() const
	{
		return false;
	};

	int							NewTrail( idVec3 pt, int duration );
	void						UpdateTrail( int index, idVec3 pt );
	void						DrawTrail( int index, const struct renderEntity_s* ent, srfTriangles_t* tri, float globalAlpha );
};

/*
===============================================================================

	Lightning model

===============================================================================
*/

class idRenderModelLightning : public idRenderModelStatic
{
public:
	virtual dynamicModel_t		IsDynamicModel() const;
	virtual bool				SupportsBinaryModel()
	{
		return false;
	}
	virtual bool				IsLoaded() const;
	virtual idRenderModel* 		InstantiateDynamicModel( const struct renderEntity_s* ent, const viewDef_t* view, idRenderModel* cachedModel );
	virtual idBounds			Bounds( const struct renderEntity_s* ent ) const;

	// with the addModels2 arrangement we could have light accepting and
	// shadowing dynamic models, but the original game never did
	virtual bool				ModelHasDrawingSurfaces() const
	{
		return true;
	};
	virtual bool				ModelHasInteractingSurfaces() const
	{
		return false;
	};
	virtual bool				ModelHasShadowCastingSurfaces() const
	{
		return false;
	};
};

/*
================================================================================

	idRenderModelSprite

================================================================================
*/
class idRenderModelSprite : public idRenderModelStatic
{
public:
	virtual	dynamicModel_t		IsDynamicModel() const;
	virtual bool				SupportsBinaryModel()
	{
		return false;
	}
	virtual	bool				IsLoaded() const;
	virtual	idRenderModel* 		InstantiateDynamicModel( const struct renderEntity_s* ent, const viewDef_t* view, idRenderModel* cachedModel );
	virtual	idBounds			Bounds( const struct renderEntity_s* ent ) const;

	// with the addModels2 arrangement we could have light accepting and
	// shadowing dynamic models, but the original game never did
	virtual bool				ModelHasDrawingSurfaces() const
	{
		return true;
	};
	virtual bool				ModelHasInteractingSurfaces() const
	{
		return false;
	};
	virtual bool				ModelHasShadowCastingSurfaces() const
	{
		return false;
	};
};

#endif /* !__MODEL_LOCAL_H__ */
