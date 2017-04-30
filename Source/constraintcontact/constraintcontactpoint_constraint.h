#ifndef __constraintcontactpoint_CONSTRAINT_H__
#define __constraintcontactpoint_CONSTRAINT_H__
/***************************************************************************************
 
 2017 Paulo Scatena

 Educational purposes only.
 
 Inspired by:
 
 Autodesk(R) Open Reality(R) Samples
 (C) 2009 Autodesk, Inc. and/or its licensors

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
 IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY 
 AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER 
 IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
 OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

***************************************************************************************/

/**	\file	constraintcontactpoint_constraint.h
*	Declaration of a simple constraint class.
*	Simple constraint class declaration (FBSimpleConstraint).
*/

//--- SDK include
#include <fbsdk/fbsdk.h>

#define	ORCONSTRAINT__CLASSNAME		constraintcontactpoint
#define ORCONSTRAINT__CLASSSTR		"constraintcontactpoint"


//checar o que posso apagar
#define ORSKELETON__CLASSSTR	"FBModelSkeleton"
#define COPY_NAMESPACE			"Copy"
#define COPY_NAMESPACE_DOT		FBString("Copy:")

#define DISPLAYALL	2
#define DISPLAYSEL	1
#define DISPLAYNIL	0

#define MAXLINK	100

//! A simple constraint class.
class constraintcontactpoint : public FBConstraint
{
	//--- declaration
	FBConstraintDeclare( constraintcontactpoint, FBConstraint );

public:
	//--- Creation & Destruction
	virtual bool			FBCreate() override;						//!< Constructor.
	virtual void			FBDestroy() override;						//!< Destructor.

	//--- Animation node management
	virtual void			RemoveAllAnimationNodes() override;			//!< Remove animation nodes.
	virtual void			SetupAllAnimationNodes() override;			//!< Setup animation nodes.

	//--- Real-Time evaluation engine function.
	virtual bool			AnimationNodeNotify		( FBAnimationNode* pAnimationNode, FBEvaluateInfo* pEvaluateInfo, FBConstraintInfo* pConstraintInfo ) override;

	//--- FBX Interface
	virtual bool			FbxStore	( FBFbxObject* pFbxObject, kFbxObjectStore pStoreWhat ) override;	//!< FBX Storage.
	virtual bool			FbxRetrieve	( FBFbxObject* pFbxObject, kFbxObjectStore pStoreWhat ) override;	//!< FBX Retrieval.

	// Root List Set-up
	void					SetRootString( FBString pLongName, FBString pShortName );
	FBString				GetRootString();

	void					SetRootNode( FBModel* pRootModel );
	

	void					SelectMarker( int pIndex );
	void					FocusMarker( int pIndex );


	void					DefocusAll(); // remove the focus on weight properties
	void					Focus( FBModel* pMarker ); //focus weight property on model
	

	//trazendo as funções do layout
	bool					SetUpAuxRig ();

	void					DeselectAll();
	


	int						GetNodeCount();
	const char*				GetNodeName( int pIndex );
	
	void					ShowMarker( int pIndex );
	FBModel*				GetMarkerAt( int pIndex );


	int						AddComponentNode( FBComponent* pComponent );

	bool					GetSetUp();

	// usando property pra ser permanente
	void					SetAutoKey( bool pBool );
	bool					GetAutoKey();

	void					SetAutoSelect( bool pBool );
	bool					GetAutoSelect();

	// controles
	void					SetSceneSelect( bool pBool );
	bool					GetSceneSelect();

	void					SetPersistentSelect( bool pBool );
	bool					GetPersistentSelect();

	// seleção permanente
	void					SetListSelIndex( int pIndex );
	int						GetListSelIndex();

	// radio display
	void					SetDisplayMarkerType( int pType );
	int						GetDisplayMarkerType();

	void					SetMarkersVisibility( bool pBool );

	// seleção de all keyframes
	void					SetFindAllKeyframes( bool pBool );
	bool					GetFindAllKeyframes();

	// find 
	int						FindWeight();
	int						FindNextKey();
	int						FindPrevKey();


	// overwrite de array

	void					DeleteNodeAt( int pIndex );	


	void					ResetTool();

private:

	// funções pro marker marker
	FBModel*				CreateMarker(char* pName );
	
	FBModel*				FindRootSrc();
	FBModel*				FindRootDst();

	void				AutoKey();
	void				AutoKeyOthers( FBTime pTime, FBInterpolation pInterp, FBTangentMode pTangen, double pWeight );



	void					SnapModel( FBModel* pSrc, FBModel* pDst );
	FBModel*				NewLinkedMarker( FBModel* pParentModel );


	void					SetupAnimationNode();
	void					ReSetupAnimationNode();

	bool					RegisterNode( FBModel* pLocalModel, FBModel* pTargetModel );


	void					SelectSkeletonHierarchy( FBModel* topNode );
	void					SelectParentChain( FBModel* pModel );

	void					DeleteHierarchy( FBModel* topNode );

	bool					FileTempSaveSelect();

	bool					FileMergeBack();
	
	void					ColorSkeletonBranch( FBModelSkeleton* topNode );

	
	FBModel*				GetTopParent( FBModel* pModel );
	FBNamespace*			GetParentNamespace( FBNamespace* pName );


	//botar private?
	//instances do sistema
	FBApplication	mApplication;
	FBSystem		mSystem;


	//usados no layout - UI
	FBModel*			mRootTopNode;			// the top node of source hierarchy

	// guardando a cópia
	FBModel*			mCopyTopNode;			// the top node of copy rig

	//
	FBModelNull*		mMarkerParent;			// just to group them all

	// controle interno
	bool				isFileRead;

	// usar propriedade aqu também?
	bool				isSetUp;
	void				SetSetUp( bool pActivate );

	int					RemoveArrayItemAt( FBAnimationNode** pArray, int pIndex, int pLength );

	// usando property pra ser permanente
	bool				isAutoKey;

	bool				isAutoSelect;

	// controles
	bool				isSceneSelect;


	bool				isPersistentSelect;


	// seleção permanente
	int					mListSelIndex;


	// radio display
	int					mDisplayMarkerType;

	// seleção de all keyframes
	bool				isFindAllKeyframes;













	// temp location string
	FBString		mTempSaveString;

	//global pra contador
	int					mMarkCount;				// count models

	
	FBModelList			mMarkerList;			// modelos de marcadores
	FBModelList			mSourceList;			// modelos fonte - source
	FBModelList			mUINodeList;			// lista pros modelos colocados na lista do ui


	bool				isMarkerConstrained;

	// nodes de anim
	FBAnimationNode*	mSourceMarkers[ MAXLINK ];
	FBAnimationNode*	mSourceNodes[ MAXLINK ];
	FBAnimationNode*	mMarkerWeights[ MAXLINK ];

	

	FBAnimationNode*	mSourceHip;
	FBAnimationNode*	mDestinHip;



	//lista de modelo - testando agora aqui
	FBModel*			mRootNode;
	FBModel*			mCopyNode;

	FBString			mRootString;				//!< string pro root

	FBString			mRootNameSpace;			//!< string pro root namespace

	/* -- Development helper function
	void				DebugIntMessage( int pNumber );
	*/

};

#endif	/* __constraintcontactpoint_CONSTRAINT_H__ */
