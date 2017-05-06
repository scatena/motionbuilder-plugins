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


// Tool Definitions
#define	ORCONSTRAINT__CLASSNAME		constraintcontactpoint
#define ORCONSTRAINT__CLASSSTR		"constraintcontactpoint"

#define COPY_NAMESPACE			"Copy"
#define COPY_NAMESPACE_DOT		FBString("Copy:")

#define DISPLAYALL	2
#define DISPLAYSEL	1
#define DISPLAYNIL	0

#define MAXLINK	100


//! Simple constraint class.
class constraintcontactpoint : public FBConstraint
{
	//--- declaration
	FBConstraintDeclare( constraintcontactpoint, FBConstraint );

public:
	//--- Creation & Destruction
	virtual bool			FBCreate() override;						//!< Constructor.
	virtual void			FBDestroy() override;						//!< Destructor.

	//--- Animation node management
	virtual void			SetupAllAnimationNodes() override;			//!< Setup animation nodes.

	//--- Real-Time evaluation engine function.
	virtual bool			AnimationNodeNotify		( FBAnimationNode* pAnimationNode, FBEvaluateInfo* pEvaluateInfo, FBConstraintInfo* pConstraintInfo ) override;

	//--- FBX Interface
	virtual bool			FbxStore	( FBFbxObject* pFbxObject, kFbxObjectStore pStoreWhat ) override;	//!< FBX Storage.
	virtual bool			FbxRetrieve	( FBFbxObject* pFbxObject, kFbxObjectStore pStoreWhat ) override;	//!< FBX Retrieval.

	// Root Node Management
	void					SetRootNode( FBModel* pRootModel );			// Set the root node (from UI Root List)

	void					SetRootString( FBString pLongName, FBString pShortName );
	FBString				GetRootString();

	// Target Nodes Management
	int						AddComponentNode( FBComponent* pComponent );// Add a target node (from UI Models List)
	
	int						GetNodeCount();
	const char*				GetNodeName( int pIndex );

	// Target Markers Management
	FBModel*				GetMarkerAt( int pIndex );					// Return marker at index

	void					ShowMarker( int pIndex );
	void					SelectMarker( int pIndex );
	void					FocusMarker( int pIndex );					// Focus marker's weight property

	// Constraint Configuration Interface
	void					SetAutoKey( bool pBool );					// Auto Key
	bool					GetAutoKey();

	void					SetAutoSelect( bool pBool );				// Auto Select
	bool					GetAutoSelect();

	void					SetSceneSelect( bool pBool );				// Scene Select
	bool					GetSceneSelect();

	void					SetPersistentSelect( bool pBool );			// Persistent Selection
	bool					GetPersistentSelect();

	void					SetDisplayMarkerType( int pType );			// Markers' Display
	int						GetDisplayMarkerType();

	void					SetFindAllKeyframes( bool pBool );			// Keyframe Navigation Option
	bool					GetFindAllKeyframes();

	void					SetListSelIndex( int pIndex );				// Persistent Selection Information
	int						GetListSelIndex();

	bool					GetSetUp();									// Setup Status

	// Layout (UI) Helper Functions
	void					DefocusAll();								// Reset weight properties focus

	void					DeselectAll();	

	int						FindWeight();
	int						FindNextKey();								// Keyframe Navigation 
	int						FindPrevKey();

	void					SetMarkersVisibility( bool pBool );			// Show or Hide All Markers

	// Specific Helper Funcions
	bool					SetUpAuxRig ();								// Setup auxiliary rig (reference copy)

	void					DeleteNodeAt( int pIndex );					// Delete marker and node reference at index

	void					ResetTool();								// Reset Constraint

private:
	// System instances
	FBApplication			mApplication;
	FBSystem				mSystem;

	// Constraint Configuration
	bool					isAutoKey;
	bool					isAutoSelect;
	bool					isSceneSelect;
	bool					isPersistentSelect;
	int						mDisplayMarkerType;
	bool					isFindAllKeyframes;
	int						mListSelIndex;
	bool					isSetUp;

	// Models Management
	FBModel*				mRootNode;									// Root of source hierarchy

	FBString				mRootString;								// Root name

	FBString				mRootNameSpace;								// Root namespace

	FBModel*				mCopyNode;									// Root of copied hierarchy

	FBModel*				mRootTopNode;								// Top node of source hierarchy

	FBModel*				mCopyTopNode;								// Top node of copied hierarchy

	FBModelNull*			mMarkerParent;								// Group to serve as parent for new markers

	FBModelList				mSourceList;								// Source Nodes List
	FBModelList				mMarkerList;								// Markers List
	FBModelList				mUINodeList;								// UI Nodes List

	// Animation Nodes Management
	FBAnimationNode*		mSourceHip;									// Source root node

	FBAnimationNode*		mDestinHip;									// Destination root node

	FBAnimationNode*		mSourceMarkers[ MAXLINK ];					// Source markers nodes array
	FBAnimationNode*		mSourceNodes[ MAXLINK ];					// Source models nodes array
	FBAnimationNode*		mMarkerWeights[ MAXLINK ];					// Marker weights nodes array

	// General Management
	bool					isFileRead;									// Auxiliary for tool reconstruction on file read

	FBString				mTempSaveString;							// Temporary location for saving

	int						mMarkCount;									// Counter for created markers

	bool					isMarkerConstrained;						// Auxiliary for tool activation

	// Application Save/Merge Functions
	bool					FileTempSaveSelect();						// Save selection to a temporary file

	bool					FileMergeBack();							// Merge temporary file back into the scene

	// Auxiliary Management Functions 
	FBModel*				GetTopParent( FBModel* pModel );			// Get the top parent of a model

	FBNamespace*			GetParentNamespace( FBNamespace* pName );	// Get the top namespace of a model

	FBModel*				FindRootSrc();								// Get the constraint source root model

	FBModel*				FindRootDst();								// Get the constraint destination root model

	void					SetSetUp( bool pActivate );					// Register set up status

	// Markers Management Functions
	bool					RegisterNode( FBModel* pLocalModel, FBModel* pTargetModel );	// Functions for creating...
	FBModel*				NewLinkedMarker( FBModel* pParentModel );						// ...and configuring..
	FBModel*				CreateMarker(char* pName );										// ...new markers.

	void					SnapModel( FBModel* pSrc, FBModel* pDst );	// Snaps one model to the position of the other

	void					AutoKey();									// Create and set up keys for created markers
	void					AutoKeyOthers( FBTime pTime, FBInterpolation pInterp, FBTangentMode pTangen, double pWeight );

	void					Focus( FBModel* pMarker );					// Focus markers's weight property

	// Animation Nodes Management
	void					SetupAnimationNode();						// Set up nodes for real-time evaluation (progressive)
	void					ReSetupAnimationNode();						// Set up nodes for real-time evaluation (one-shot)

	int						RemoveArrayItemAt( FBAnimationNode** pArray, int pIndex, int pLength ); // Delete item from nodes array

	// Hierachy Management Functions
	void					SelectSkeletonHierarchy( FBModel* topNode );// Recursive function for hierarchy selection
	void					SelectParentChain( FBModel* pModel );		// Recursive function for parents selection

	void					ColorSkeletonBranch( FBModelSkeleton* topNode );// Recursive function for colorizing skeleton hierarchy

	void					DeleteHierarchy( FBModel* topNode );		// Recursive function for deleting model and its children

	/* -- Development helper function
	void				DebugIntMessage( int pNumber );
	*/
};

#endif	/* __constraintcontactpoint_CONSTRAINT_H__ */
