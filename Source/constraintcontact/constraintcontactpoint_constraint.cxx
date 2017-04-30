/***************************************************************************************
 
 2017 Paulo Scatena

 Educational purposes only.
 
 Inspired by:
 
 Autodesk(R) Open Reality(R) Samples
 (C) 2009 Autodesk, Inc. and/or its licensors

 Open license for redistribution and use is under negotiation.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
 IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY 
 AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER 
 IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
 OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

***************************************************************************************/

/**	\file	constraintcontactpoint_constraint.cxx
*	Definition of a simple constraint class.
*	Simple constraint function definitions for the FBSimpleConstraint
*	class.
*/

// Version
#define CONSTRAINT_VERSION_01		1003

//--- Class declarations
#include "constraintcontactpoint_constraint.h"
#include "constraintcontactpoint_layout.h"

//--- Registration defines
#define	ORCONSTRAINT__CLASS		ORCONSTRAINT__CLASSNAME
#define ORCONSTRAINT__NAME		"Contact Point Constraint"
#define	ORCONSTRAINT__LABEL		"Contact Point"
#define ORCONSTRAINT__DESC		"Contact Point Description"
#define ORCONSTRAINT__ICON		"devices_ultratrak.png"



//--- implementation and registration
FBConstraintImplementation	(	ORCONSTRAINT__CLASS		);
FBRegisterConstraint		(	ORCONSTRAINT__NAME,
								ORCONSTRAINT__CLASS,
								ORCONSTRAINT__LABEL,
								ORCONSTRAINT__DESC,
								ORCONSTRAINT__ICON		);	// Icon filename (ultra trak icon) // default was FB_DEFAULT_SDK_ICON


/************************************************
 *	Creation function.
 ************************************************/
bool constraintcontactpoint::FBCreate()
{
	/*
	*	1. Create Reference group & elements
	*	2. Set constraint variables (deformer,layout,description)
	*	3. Set pointers to NULL if necessary
	*/

	Deformer	= false;
	HasLayout	= true;
	Description = "Contact Point Constraint";


	// variables
	mMarkCount = 0;
	isMarkerConstrained = false;
	
	// Pointers
	mSourceMarkers[0] = NULL;
	mSourceNodes[0] = NULL;
	mMarkerWeights[0] = NULL;

	mRootTopNode = NULL;
	mCopyTopNode = NULL;

	// internal control
	isFileRead = false;

	//UI helpers
	isSetUp = false;
	//mRootString = "";
	//initialize
	isAutoKey = true;
	isAutoSelect = true;
	isSceneSelect = true;
	isPersistentSelect = true;
	isFindAllKeyframes = true;
	mListSelIndex = -1;
	mDisplayMarkerType = DISPLAYSEL;

	// cria o skeleton pra poupar tempo
	FBModel* skeModel = new FBModelSkeleton( "Node 1" );
	skeModel->Show = true;

	return true;
}

/************************************************
 *	Destruction function.
 ************************************************/
void constraintcontactpoint::FBDestroy()
{
	/*
	*	Free any user memory associated to constraint
	*/

}


/************************************************
 *	Setup all of the animation nodes.
 ************************************************/
void constraintcontactpoint::SetupAllAnimationNodes()
{
	/*
	*	- Based on the existence of the references (ReferenceGet() function),
	*	  create the required animation nodes.
	*	- A source will use an Out node, whereas a destination will use
	*	  an In Node.
	*	- If the node is with respect to a deformation, then Bind the reference
	*	  to the deformation notification ( DeformerBind() )
	*/
	
	// Here the animation nodes are set up only if constraint was read from file.
	// Otherwise, they are progressively created with SetupAnimationNode()
	
	if ( isFileRead )
	{
		ReSetupAnimationNode();
		isFileRead = false;
	}

}


/************************************************
 *	Removed all of the animation nodes.
 ************************************************/
void constraintcontactpoint::RemoveAllAnimationNodes()
{
	/*
	*	If nodes have been bound to deformations, remove the binding.
	*/
}




/************************************************
 *	Real-Time Engine Evaluation.
 ************************************************/
bool constraintcontactpoint::AnimationNodeNotify(FBAnimationNode* pConnector,FBEvaluateInfo* pEvaluateInfo,FBConstraintInfo* pConstraintInfo)
{
	/*
	*	Constraint Evaluation
	*	1. Read data from sources
	*	2. Calculate necessary operations.
	*	3. Write output to destinations
	*	Note: Not the deformation operations !
	*/


	if (isMarkerConstrained)
	{
		// new vector, zeroed
		double lDestPos[3];
		/*
		for(int j=0;j<3;j++)
			lDestPos[j] = 0;
		*/

		double total_weight;
		total_weight = 0;
		// loop pra peso
		for(int i=0;i<mMarkCount;i++)
		{
			double lWeight[1];
			mMarkerWeights[i]->ReadData	( lWeight, pEvaluateInfo );
			total_weight += lWeight[0];
		}
		// make it be minimum 100
		if ( total_weight < 100 )
			total_weight = 100.0;

		//outside loop
		double lHipsPos[3];
		if( mSourceHip )
		{
			mSourceHip->ReadData		( lHipsPos, pEvaluateInfo );
			//manipulate
				for(int j=0;j<3;j++)
					lDestPos[j] = lHipsPos[j];
		}
		//loop
		for(int i=0;i<mMarkCount;i++)
		{
			double lMarkPos[3];
			double lNodePos[3];
			double lWeight[1];

			// Safety check - ultimately unnecessary
			if( mSourceNodes[i] && mSourceMarkers[i] && mMarkerWeights[i] )
			{
				mSourceNodes[i]->ReadData	( lNodePos, pEvaluateInfo );
				//( (FBAnimationNode*) mSourceNodesList->GetAt(i) )->ReadData	( lNodePos, pEvaluateInfo );
				mSourceMarkers[i]->ReadData	( lMarkPos, pEvaluateInfo );
				//( (FBAnimationNode*) mSourceMarkersList->GetAt(i) )->ReadData	( lMarkPos, pEvaluateInfo );
				mMarkerWeights[i]->ReadData	( lWeight, pEvaluateInfo );
				//( (FBAnimationNode*) mMarkerWeightsList->GetAt(i) )->ReadData	( lWeight, pEvaluateInfo );

				//manipulate
				for(int j=0;j<3;j++)
				{
					lDestPos[j] += lWeight[0]/total_weight * ( lMarkPos[j] - lNodePos[j] );
					//lDestPos[j] = lMarkPos[j];// - lNodePos[j];

					/*
					// keep values
					mPosition[i][j] = lDestPos[j];
					*/
				}

				/* hold write
				//write				
				mDestinHip->WriteData	( lDestPos, pEvaluateInfo );
				*/
			}
		}
		// write sum
		if ( mDestinHip )
			mDestinHip->WriteData	( lDestPos, pEvaluateInfo );


	}
	/*double lPosition[3];

	if( mSourceTranslation && mConstrainedTranslation )
	{
		mSourceTranslation->ReadData		( lPosition, pEvaluateInfo );
		mConstrainedTranslation->WriteData	( lPosition, pEvaluateInfo );
	}
	mPosition = lPosition;
	*/

	return true;
	

}



#define CONSTRAINT_FBX_VERSION		"Version"
#define	CONSTRAINT_FBX_PROPERTIES	"Properties"
#define CONSTRAINT_FBX_MARKERLIST	"MarkerList"
#define CONSTRAINT_FBX_SOURCELIST	"SourceList"
#define CONSTRAINT_FBX_UINODELIST	"UINodeList"
#define CONSTRAINT_FBX_SETTINGS		"Settings"
/************************************************
 *	FBX Storage.
 ************************************************/

bool constraintcontactpoint::FbxStore( FBFbxObject* pFbxObject, kFbxObjectStore pStoreWhat )
{
	/*
	*	FBX Storage of constraint parameters.
	*/

	// Storing data for the tool to be persistent
	// (instead of several properties, as was before)

	if (pStoreWhat & kAttributes)
	{
		// Write version
		pFbxObject->FieldWriteI( CONSTRAINT_FBX_VERSION, CONSTRAINT_VERSION_01 );

		// Write settings
		pFbxObject->FieldWriteBegin( CONSTRAINT_FBX_SETTINGS );
		{
			pFbxObject->FieldWriteI( isSetUp );
			pFbxObject->FieldWriteI( isAutoKey );
			pFbxObject->FieldWriteI( isAutoSelect );
			pFbxObject->FieldWriteI( isSceneSelect );
			pFbxObject->FieldWriteI( isPersistentSelect );
			pFbxObject->FieldWriteI( isFindAllKeyframes );
			pFbxObject->FieldWriteI( mDisplayMarkerType );
		}
		pFbxObject->FieldWriteEnd();

		// Write properties
		pFbxObject->FieldWriteBegin( CONSTRAINT_FBX_PROPERTIES );
		{
			// Write Markers Count
			pFbxObject->FieldWriteI( mMarkCount );

			// Root objects
			pFbxObject->FieldWriteObjectReference( mRootTopNode );
			pFbxObject->FieldWriteObjectReference( mCopyTopNode );
			pFbxObject->FieldWriteObjectReference( mRootNode );
			pFbxObject->FieldWriteObjectReference( mCopyNode );
		}
		pFbxObject->FieldWriteEnd();

		// Iterate and write lists
		for( int i=0; i<mMarkCount; i++ )
		{
			pFbxObject->FieldWriteObjectReference( CONSTRAINT_FBX_MARKERLIST, mMarkerList.GetAt(i) );
			pFbxObject->FieldWriteObjectReference( CONSTRAINT_FBX_SOURCELIST, mSourceList.GetAt(i) );
			pFbxObject->FieldWriteObjectReference( CONSTRAINT_FBX_UINODELIST, mUINodeList.GetAt(i) );
		}
	}

	return true;
}


/************************************************
 *	FBX Retrieval.
 ************************************************/
bool constraintcontactpoint::FbxRetrieve( FBFbxObject* pFbxObject, kFbxObjectStore pStoreWhat )
{
	/*
	*	FBX Retrieval of constraint parameters.
	*/

	// using FBXRetrieve to call for the rebuild
	
	if (pStoreWhat & kAttributes) // conditional to avoid multiple readings?
	{
		int	lVersion = 0;
		//int	mCount = 0;

		lVersion = pFbxObject->FieldReadI( CONSTRAINT_FBX_VERSION );
		if ( lVersion <= CONSTRAINT_VERSION_01 )
		{
			// Safety warning
			if ( lVersion < CONSTRAINT_VERSION_01 )
			{
				if ( FBMessageBox("Warning", "File was configured with an older version of Contact Editing plugin.\nFunctionality might be compromised.", "OK", "Abort", NULL, 1 ) == 2 )
					return false;
			}
			// Read settings
			if( pFbxObject->FieldReadBegin ( CONSTRAINT_FBX_SETTINGS ) )
			{
				isSetUp = pFbxObject->FieldReadI() == 1;
				isAutoKey = pFbxObject->FieldReadI() == 1;
				isAutoSelect = pFbxObject->FieldReadI() == 1;
				isSceneSelect = pFbxObject->FieldReadI() == 1;
				isPersistentSelect = pFbxObject->FieldReadI() == 1;
				isFindAllKeyframes = pFbxObject->FieldReadI() == 1;
				mDisplayMarkerType = pFbxObject->FieldReadI();
				pFbxObject->FieldReadEnd();
			}
			// Read Properties
			if(pFbxObject->FieldReadBegin( CONSTRAINT_FBX_PROPERTIES ))
			{
				mMarkCount = pFbxObject->FieldReadI();
				mRootTopNode = (FBModel*) FBGetFBComponent( pFbxObject->FieldReadObjectReference() );
				mCopyTopNode = (FBModel*) FBGetFBComponent( pFbxObject->FieldReadObjectReference() );
				SetRootNode( (FBModel*) FBGetFBComponent( pFbxObject->FieldReadObjectReference() ) );
				mCopyNode = (FBModel*) FBGetFBComponent( pFbxObject->FieldReadObjectReference() );
				pFbxObject->FieldReadEnd();
			}
			// Iterate and populate
			for( int i=0; i<mMarkCount; i++ )
			{
				// get model and populate list
				mMarkerList.Add( (FBModel*) FBGetFBComponent( pFbxObject->FieldReadObjectReference( CONSTRAINT_FBX_MARKERLIST ) ) );
				mSourceList.Add( (FBModel*) FBGetFBComponent( pFbxObject->FieldReadObjectReference( CONSTRAINT_FBX_SOURCELIST ) ) );
				mUINodeList.Add( (FBModel*) FBGetFBComponent( pFbxObject->FieldReadObjectReference( CONSTRAINT_FBX_UINODELIST ) ) );
			}
			// Allow for recreating animation nodes
			if ( isSetUp )
				isFileRead = true;
		}
		// Incompatibility warning
		else
		{
			FBMessageBox("Warning", "File was configured with a newer version of Contact Editing plugin.\nInstall newer version or contact developer.", "OK", NULL, NULL, 1 );
			return false;
		}
	}
	return true;
}





//   #########################
//   #########################
//		Helpers
//   #########################
//   #########################

//setagem de listas
//funções de setagem lista root
void constraintcontactpoint::SetRootNode( FBModel* pRootModel )
{
	mRootNode = pRootModel;
	SetRootString( (FBString)pRootModel->LongName, (FBString)pRootModel->Name );
}

void constraintcontactpoint::SetRootString( FBString pLongName, FBString pShortName )
{	
	// set root string
	mRootString = pLongName;

	// set namespace
	int lNameLength = pShortName.GetLen();
	int lFullNameLength = pLongName.GetLen();
	//get everything to the left
	mRootNameSpace = pLongName.Left( lFullNameLength - lNameLength );
}

FBString constraintcontactpoint::GetRootString()
{	
	return mRootString;
}




int constraintcontactpoint::AddComponentNode( FBComponent* pComponent )
{
	// if Null
	if ( !pComponent )
		return 0;
	
	// if is skeleton
	if( pComponent->Is( FBModelSkeleton::TypeInfo ) )
	{
		// ver se está configurado - se já tem node pai
		if ( isSetUp )
		{
			// Get top of hierarchy to see where it comes from
			FBModel* lTopNode = GetTopParent( (FBModel*) pComponent );
			if ( lTopNode == mRootTopNode )	// Dragged model is part of original hierachy
			{
				// Find parallel model in "copy" hierarchy
				FBModel * lCopyModel = FBFindModelByLabelName( FBString(COPY_NAMESPACE_DOT + mRootNameSpace + pComponent->Name) );
				if ( lCopyModel )
				{
					// Register it - parallel model as node reference and dragged node as marker target
					if ( RegisterNode( lCopyModel, (FBModel*) pComponent ) )
					{
						// Add item to nodes list
						mUINodeList.Add( (FBModel*) pComponent );
					
						// Added
						return 1;
					}
				}
				// Else, parallel model was not found
				FBMessageBox("Warning", "Could not find equivalent node in parallel copy.\n(Probably due to naming conflicts)\nTry adding the bone directly from copied hierarchy.", "OK", NULL, NULL, 1 );

			}
			else if ( lTopNode == mCopyTopNode ) // Dragged model is part of copy hierachy
			{
				// Register it - dragged node both as reference and as marker target
				if ( RegisterNode( (FBModel*) pComponent, (FBModel*) pComponent ) )
				{
					// Add item to nodes list
					mUINodeList.Add( (FBModel*) pComponent );
				
					// Added
					return 1;
				}
			}
			// Else, dragged model has no relation to the hierarchy - ignore it
		}
	}
	return 0;
}




FBModel* constraintcontactpoint::CreateMarker(char* pName )
{
	// New Marker
	FBModel* lMarker = new FBModelMarker( pName );

	// adjust size
	double lSize = 300;
	FBProperty* lPproperty = lMarker->PropertyList.Find("Size");
	lPproperty->SetData(&lSize);
	
	// adjust look
	int lLook = 2;
	lPproperty = lMarker->PropertyList.Find("Look");
	lPproperty->SetData(&lLook);
	
	// count 
	mMarkCount += 1;

	return lMarker;
}


void constraintcontactpoint::SnapModel( FBModel* pDst, FBModel* pSrc )
{
	FBVector3d l3dGlobalTrans;
	//true for global
	pSrc->GetVector(l3dGlobalTrans, kModelTranslation, true ); 
    pDst->Translation = l3dGlobalTrans;

}

FBModel* constraintcontactpoint::NewLinkedMarker( FBModel* pParentModel )
{
	if ( mMarkCount == 100 )
	{
		// do not allow more
		return NULL;
	}


	// nome - padding
	char paddednumc[10];
	sprintf( paddednumc, "%02d", mMarkCount );
	
	FBString pName = FBString("Mark") + FBString(paddednumc) + FBString("_") + pParentModel->Name ;
	// create the marker
	FBModel* lMarker = CreateMarker( pName );
	
	// if it is the first one, create a parent node
	if ( mMarkCount == 1 )
		mMarkerParent = new FBModelNull("MarkersGrp");

	// make the new marker a child of it
	lMarker->Parent = mMarkerParent;

	// snapa o marker pro modelo
	SnapModel( lMarker, pParentModel );

	// adiciona propriedade
	FBPropertyAnimatable* lProp = (FBPropertyAnimatable*)lMarker->PropertyCreate("Weight", kFBPT_int, "Integer", true, true, NULL);

	// set minimum and max and clamp - alt SetMinMax
	lProp->SetMin(0, true);
	lProp->SetMax(100, true);
	// SetAnimated exposes the property to the relations constraint
	lProp->SetAnimated(true);

	// adiciona na lista
	mMarkerList.Add(lMarker);

	return lMarker;

}



FBModel* constraintcontactpoint::FindRootSrc()
{
	//FBModel* lModel = FBFindModelByLabelName( mRootString );
	//FBModel* lModel = FBFindModelByLabelName( COPY_NAMESPACE_DOT + mRootString );
	return mCopyNode;
}

FBModel* constraintcontactpoint::FindRootDst()
{
	//FBModel* lModel = FBFindModelByLabelName( COPY_NAMESPACE_DOT + mRootString );
	//FBModel* lModel = FBFindModelByLabelName( mRootString );
	return mRootNode;
}




void constraintcontactpoint::SetAutoKey( bool pBool )
{
	isAutoKey = pBool;
}
bool constraintcontactpoint::GetAutoKey()
{
	return isAutoKey;
}


void constraintcontactpoint::SetAutoSelect( bool pBool )
{
	isAutoSelect = pBool;
}
bool constraintcontactpoint::GetAutoSelect()
{
	return isAutoSelect;
}

void constraintcontactpoint::SetSceneSelect( bool pBool )
{
	isSceneSelect = pBool;
}
bool constraintcontactpoint::GetSceneSelect()
{
	return isSceneSelect;
}

void constraintcontactpoint::SetPersistentSelect( bool pBool )
{
	isPersistentSelect = pBool;
}
bool constraintcontactpoint::GetPersistentSelect()
{
	return isPersistentSelect;
}


void constraintcontactpoint::SetFindAllKeyframes( bool pBool )
{
	isFindAllKeyframes = pBool;
}
bool constraintcontactpoint::GetFindAllKeyframes()
{
	return isFindAllKeyframes;
}


void constraintcontactpoint::SetListSelIndex( int pIndex )
{
	mListSelIndex = pIndex;
}
int constraintcontactpoint::GetListSelIndex()
{
	return mListSelIndex;
}

void constraintcontactpoint::SetDisplayMarkerType( int pType )
{
	mDisplayMarkerType = pType;
}
int constraintcontactpoint::GetDisplayMarkerType()
{
	return mDisplayMarkerType;
}

void constraintcontactpoint::SetMarkersVisibility( bool pBool )
{
	int lCnt = mMarkerList.GetCount();
	for( int i=0; i<lCnt; i++ )
	{
		mMarkerList.GetAt( i )->Show = pBool;
	}
}


int	constraintcontactpoint::GetNodeCount()
{
	return mUINodeList.GetCount();
}

const char*	constraintcontactpoint::GetNodeName( int pIndex )
{
	return mUINodeList[pIndex]->LongName;
}

void constraintcontactpoint::ShowMarker( int pIndex )
{
	mMarkerList.GetAt( pIndex )->Show = true;
}

FBModel* constraintcontactpoint::GetMarkerAt( int pIndex )
{
	return mMarkerList.GetAt( pIndex );
}



//   #########################
//   #########################
//		Buttons
//   #########################
//   #########################



bool constraintcontactpoint::RegisterNode( FBModel* pLocalModel, FBModel* pTargetModel )
{
	// Create a marker - configure if created
	if ( NewLinkedMarker( pTargetModel ) )
	{
		// Append to list
		mSourceList.Add( pLocalModel );

		// Set up animation node
		SetupAnimationNode();

		// Set key if set to do so
		if ( isAutoKey )
		{
			AutoKey();
		}
		// Check if new marker will be displayed or not
		if ( mDisplayMarkerType == DISPLAYALL )
		{
			mMarkerList[mMarkCount-1]->Show = true;
		}
		// Check whether to select (in list) or nor
		if ( isAutoSelect )
		{
			// Select last added
			SetListSelIndex( mMarkCount - 1 );
			// Check whether to select (in scene)
			if ( isSceneSelect )
			{
				// Deselect other, select current
				DeselectAll();
				mMarkerList[mMarkCount-1]->Selected = true;
			}
			// Check whether to display
			if ( mDisplayMarkerType == DISPLAYSEL )
			{
				// Hide others, show current
				SetMarkersVisibility( false );
				mMarkerList[mMarkCount-1]->Show = true;
			}
		}
		return true;
	}
	else // Faced a problem or marker count limit
	{
		return false;
	}
}

void constraintcontactpoint::SetupAnimationNode()
{
	// Here we set up the animation nodes (which will be constantly evaluated real-time)
	
	// On the first run, add nodes for source and destination root models
	if (mMarkCount==1)
	{
		// Source root node
		FBModel* srcModel = FindRootSrc();
		if ( srcModel )
			mSourceHip = AnimationNodeOutCreate( 0, srcModel, ANIMATIONNODE_TYPE_TRANSLATION );
		else
			FBMessageBox("Tool error", "Reference root was not found", "OK", NULL, NULL, 1 );
		// Destination root node
		FBModel* dstModel = FindRootDst();
		if ( dstModel )
			mDestinHip = AnimationNodeInCreate ( 1, dstModel, ANIMATIONNODE_TYPE_TRANSLATION );
		else
			FBMessageBox("Tool error", "Target root was not found", "OK", NULL, NULL, 1 );
		// Activate tool (internal bool)
		isMarkerConstrained = true;
	}
	// Safety check - if there are markers and count is consistent
	if ( mMarkCount > 0 && mMarkCount == mMarkerList.GetCount() )
	{
		// Source node (from reference hierarchy)
		FBModel* srcNode = mSourceList.GetAt( mMarkCount-1 );
		if ( srcNode )
		{
			mSourceNodes[mMarkCount-1]	= AnimationNodeOutCreate( 2, srcNode, ANIMATIONNODE_TYPE_TRANSLATION );
		}
		else
			FBMessageBox("Tool error", "Reference node was not found", "OK", NULL, NULL, 1 );
		// Marker node - for position...
		FBModel* srcMark = mMarkerList.GetAt( mMarkCount-1 );
		if ( srcMark )
		{
			mSourceMarkers[mMarkCount-1] = AnimationNodeOutCreate ( 3, srcMark, ANIMATIONNODE_TYPE_TRANSLATION );
			// ...plus its weight
			mMarkerWeights[mMarkCount-1] = AnimationNodeOutCreate ( 4, srcMark, ANIMATIONNODE_TYPE_INTEGER );
		}
		else
			FBMessageBox("Tool error", "Reference marker was not found", "OK", NULL, NULL, 1 );
	}
}

void constraintcontactpoint::ReSetupAnimationNode()
{
	//set nodes
	//hip source node
	FBModel* srcModel = FindRootSrc();
	if ( srcModel )
		mSourceHip = AnimationNodeOutCreate( 0, srcModel, ANIMATIONNODE_TYPE_TRANSLATION );
	else
		FBMessageBox("Tool error", "Reference root was not found", "OK", NULL, NULL, 1 );
	// destination node
	FBModel* dstModel = FindRootDst();
	if ( dstModel )
		mDestinHip = AnimationNodeInCreate ( 1, dstModel, ANIMATIONNODE_TYPE_TRANSLATION );
	else
		FBMessageBox("Tool error", "Target root was not found", "OK", NULL, NULL, 1 );

	//also, tell the tool now it can run - activate tool
	isMarkerConstrained = true;

	//debug
	for( int i=0; i<mMarkCount; i++ )
	{
		// source local no esqueleto
		FBModel* srcNode = mSourceList.GetAt(i);
		if ( srcNode )
		{
			mSourceNodes[i]	= AnimationNodeOutCreate( 2, srcNode, ANIMATIONNODE_TYPE_TRANSLATION );
		}
		else
			FBMessageBox("Tool error", "Reference node was not found", "OK", NULL, NULL, 1 );
		// source do marcador
		FBModel* srcMark = mMarkerList.GetAt( i );
		if ( srcMark )
		{
			mSourceMarkers[i] = AnimationNodeOutCreate ( 3, srcMark, ANIMATIONNODE_TYPE_TRANSLATION );
			mMarkerWeights[i] = AnimationNodeOutCreate ( 4, srcMark, ANIMATIONNODE_TYPE_INTEGER );
		}
		else
			FBMessageBox("Tool error", "Reference marker was not found", "OK", NULL, NULL, 1 );
	}
}

	
void constraintcontactpoint::SetSetUp ( bool pActivate )
{
	isSetUp = pActivate;
}
bool constraintcontactpoint::GetSetUp ()
{
	return isSetUp;
}

bool constraintcontactpoint::SetUpAuxRig ()
{
	//Setup auxiliary rig
	// See if exists
	//TODO
	DeselectAll();
	// Get 'root'
	if ( mRootNode )
	{
		// store the top of original hierarchy
		mRootTopNode = GetTopParent( mRootNode );

		//Select branches
		SelectSkeletonHierarchy( mRootNode );

		//Select hierarchy above
		SelectParentChain( mRootNode );

		// get local time
		FBPlayerControl				mPlayerControl; 
		FBTime localTime = FBSystem().LocalTime;


		//Duplicate via save selection 
		if ( !FileTempSaveSelect() )
			return false;

		//register setup - so ui can properly refresh
		SetSetUp ( true );

		//Import duplicate copy
		if ( !FileMergeBack() )
		{
			SetSetUp ( false );
			return false;
		}

		// go back to time it was
		mPlayerControl.Goto(localTime);
		 
		// colorize
		mCopyNode = FBFindModelByLabelName( COPY_NAMESPACE_DOT + mRootString );
		if ( mCopyNode )
		{
			ColorSkeletonBranch( (FBModelSkeleton*)mCopyNode );
			// store the top of hierarchy of the copy
			mCopyTopNode = GetTopParent( mCopyNode );
		}
		
		// activate constraint
		this->Active = true;

		return true;
	}
	// case things went bad
	SetSetUp ( false );
	return false;
}


void constraintcontactpoint::DeselectAll()
{
	// iterate over components
	FBPropertyListComponent lList = FBSystem().Scene->Components;
	int lCnt = lList.GetCount();
	for( int i=0; i<lCnt; i++ )
	{
		FBComponent* lComp = lList.GetAt(i);
		if ( lComp->Selected )
			lComp->Selected = false;
	}
}
	
void constraintcontactpoint::SelectSkeletonHierarchy( FBModel* topNode )
{
	// Recursive function !
	// if there is no model, terminate
	if ( topNode == NULL )
		return;
	
	// number of children
	int lCnt = topNode->Children.GetCount();
	for( int i=0; i<lCnt; i++ )
	{
		// if is skeleton
		if ( topNode->Children.GetAt(i)->Is( FBModelSkeleton::TypeInfo ) )
			SelectSkeletonHierarchy( (FBModel*)topNode->Children.GetAt(i) );
	}
	topNode->Selected = true;
}

void constraintcontactpoint::SelectParentChain( FBModel* pModel )
{
	// Recursive function !
	// if there is no model, terminate
	if ( pModel == NULL )
		return;

	// if parent exists...
	if ( pModel->Parent )
	{
		SelectParentChain( pModel->Parent );
		pModel->Parent->Selected = true;
	}
}



void constraintcontactpoint::DeleteHierarchy( FBModel* topNode )
{
	// Recursive function !
	// if there is no model, terminate
	if ( topNode == NULL )
		return;
	
	// delete every children back to front
	// number of children
	int lCnt = topNode->Children.GetCount() - 1;
	for( int i=0; i<=lCnt; i++ )
	{
		DeleteHierarchy( (FBModel*)topNode->Children.GetAt( lCnt-i ) );
	}
	topNode->FBDelete();
}
bool constraintcontactpoint::FileTempSaveSelect()
{
	//setting options
	FBFbxOptions lOptions = FBFbxOptions(false);
	
	lOptions.SaveSelectedModelsOnly = true;
	// disregard system stuff
    lOptions.BaseCameras = false;
    lOptions.CameraSwitcherSettings = false;
    lOptions.CurrentCameraSettings = false;
    lOptions.GlobalLightingSettings = false;
    lOptions.TransportSettings = false;


	mTempSaveString = FBString(mSystem.TempPath) + "SaveSelected.fbx";
	// Save selected models into a temp file
	bool lResult = mApplication.FileSave( mTempSaveString, &lOptions );

	if( !lResult )
		FBMessageBox("Error", "Could not duplicate structure.\nExport error.", "OK", NULL, NULL, 1 );

	return lResult;
}

bool constraintcontactpoint::FileMergeBack()
{
	//setting options
	FBFbxOptions lOptions = FBFbxOptions(false);
	// + namespace
	lOptions.NamespaceList = COPY_NAMESPACE;

	bool lResult = mApplication.FileMerge ( mTempSaveString, false, &lOptions);

	if( !lResult )
		FBMessageBox("Error", "Could not duplicate structure.\nImport error.", "OK", NULL, NULL, 1 );

	return lResult;
}
	
void constraintcontactpoint::ColorSkeletonBranch( FBModelSkeleton* topNode )
{
	// Recursive function !
	// if there is no model, terminate
	if ( topNode == NULL )
		return;
	
	// number of children
	int lCnt = topNode->Children.GetCount();
	for( int i=0; i<lCnt; i++ )
	{
		// if is skeleton
		if ( topNode->Children.GetAt(i)->Is( FBModelSkeleton::TypeInfo ) )
			ColorSkeletonBranch( (FBModelSkeleton*)topNode->Children.GetAt(i) );
	}
	topNode->Color = FBColor(0,0.666,1);
}

FBModel* constraintcontactpoint::GetTopParent( FBModel* pModel )
{
	if ( pModel )
	{
		if ( pModel->Parent )
			return GetTopParent( pModel->Parent );
		else
			return pModel;
	}
	// case none, return none
	return NULL;
}

FBNamespace* constraintcontactpoint::GetParentNamespace( FBNamespace* pName )
{
	if ( pName )
	{
		if ( pName->GetOwnerNamespace() )
			return GetParentNamespace( pName->GetOwnerNamespace() );
		else
			return pName;
	}
	// case none, return none
	return NULL;
}

/************************************************
 *	Create and set up keys for created marker at the moment ( auxiliary to RegisterNode )
 ************************************************/
void constraintcontactpoint::AutoKey()
{
	// Getting the weight node..
	if ( mMarkCount > 0 )
	{
		// ..via created node
		FBAnimationNode* customNode = mMarkerWeights[mMarkCount-1];
		if ( customNode )
		{
			// Key creation options
			FBInterpolation lInterp = kFBInterpolationConstant; // auto -> kFBInterpolationCubic 
			FBTangentMode lTangen = kFBTangentModeAuto; // auto-> kFBTangentModeClampProgressive;
			// Check current time to see if it needs previous zero key or not
			FBTime lStart = ((FBTimeSpan)FBSystem().CurrentTake->LocalTimeSpan).GetStart();
			FBTime lCurre = FBSystem().LocalTime;
			// Will set keys with KeyAdd directly on the node
			double lWeight;
			if ( lCurre != lStart )
			{
				// If time is different, create a zero-weight key at start
				lWeight = 0;
				FBTVector	Pos;
				customNode->KeyAdd(lStart, &lWeight, lInterp, lTangen);
			}
			// Create full-weight key at current time
			lWeight = 100;
			customNode->KeyAdd(lCurre, &lWeight, lInterp, lTangen);
			// If there are others markers, might have to add zero-weight keys
			if ( mMarkCount > 1 )
			{
				AutoKeyOthers( lCurre, lInterp, lTangen, 0.0 );
				// nah
				// customNode = mMarkerWeights[mMarkCount-2];
			}
		}
	}
}

/************************************************
 *	Create zero-weight keys on any non-zero weighted marker at the moment ( auxiliary to AutoKey )
 ************************************************/
void constraintcontactpoint::AutoKeyOthers( FBTime pTime, FBInterpolation pInterp, FBTangentMode pTangen, double pWeight )
{
	// Find nodes with non-zero weight
	for( int i=0; i<mMarkCount-1; i++ )
	{
		// Read weight from read property
		FBModel* lMarker = mMarkerList.GetAt(i);
		if ( lMarker->PropertyList.Find("Weight") )
		{
			int lWeight;
			lMarker->PropertyList.Find("Weight")->GetData(&lWeight, sizeof(lWeight));
			// Set key if weight is non-zero
			if ( lWeight > 0 )
			{
				// Grab animation node, set key
				FBAnimationNode* customNode = mMarkerWeights[i];
				if ( customNode )
				{
					customNode->KeyAdd(pTime, &pWeight, pInterp, pTangen);
				}
			}
		}
	}
}



void constraintcontactpoint::SelectMarker( int pIndex )
{
	if ( mMarkerList[pIndex] )
	{
		DeselectAll();
		mMarkerList[pIndex]->Selected = true;
	}
}

void constraintcontactpoint::FocusMarker( int pIndex )
{
	if ( mMarkerList[pIndex] )
	{
		DefocusAll();
		Focus( mMarkerList[pIndex] );
	}
}

void constraintcontactpoint::DefocusAll()
{
	// iterate and defocus
	for(int i=0;i<mMarkCount && mMarkCount == mMarkerList.GetCount();i++)
	{
		FBModel* lMarker = mMarkerList.GetAt(i);
		if ( lMarker->PropertyList.Find("Weight") )
		{

			( (FBPropertyAnimatable*)lMarker->PropertyList.Find("Weight") )->SetFocus(false);
		}
	}
}

void constraintcontactpoint::Focus( FBModel* pMarker )
{
	if ( pMarker->PropertyList.Find("Weight") )
	{
		( (FBPropertyAnimatable*)pMarker->PropertyList.Find("Weight") )->SetFocus(true);
	}
}

int constraintcontactpoint::FindWeight()
{
	int maxWeight = 0;
	int maxIndex = -1;
	for( int i=0; i<mMarkCount; i++ )
	{
		/* via read property */
		FBModel* lMarker = mMarkerList.GetAt(i);
		if ( lMarker->PropertyList.Find("Weight") )
		{
			int lWeight;
			lMarker->PropertyList.Find("Weight")->GetData(&lWeight, sizeof(lWeight));
			if ( lWeight > maxWeight )
			{
				maxWeight = lWeight;
				maxIndex = i;
			}
		}
	}

	return maxIndex;
}


int constraintcontactpoint::FindNextKey()
{
	bool includeZero = isFindAllKeyframes;
	FBTime localTime = FBSystem().LocalTime;
	FBTime minDelta = FBTime().OneHour;
	int minIndex = -1;



	// iterate over markers
	for( int i=0; i<mMarkCount; i++ )
	{
		//via fcurve
		FBAnimationNode* lNode = mMarkerWeights[i];
		FBFCurve* lFCurve = lNode->FCurve;
		//iterate over keys
		int lCount = lFCurve->Keys.GetCount();
		for( int j=0; j<lCount; j++ )
		{
			FBFCurveKey lKey = lFCurve->Keys[j];
			// index = j
			// if comes later
			if ( (FBTime)lKey.Time > localTime )
			{
				// se value =/= zero ou value == 0 && include zero
				if ( lKey.Value != 0 || includeZero )
				{
					FBTime lDelta = (FBTime)lKey.Time - localTime;
					if ( lDelta < minDelta )
					{
						minDelta = lDelta;
						minIndex = i; // index do marker
						// j daria o index do key
						// sai do loop pq as próximas serão mais distantes
						break;
					}
				}
			}
		}
	}
	return minIndex;
}

int constraintcontactpoint::FindPrevKey()
{
	bool includeZero = isFindAllKeyframes;
	FBTime localTime = FBSystem().LocalTime;
	FBTime minDelta = FBTime().OneHour;
	int minIndex = -1;

	// iterate over markers
	for( int i=0; i<mMarkCount; i++ )
	{
		//via fcurve
		FBAnimationNode* lNode = mMarkerWeights[i];
		FBFCurve* lFCurve = lNode->FCurve;
		//iterate over keys - reversed order
		int lCount = lFCurve->Keys.GetCount();
		for( int j=lCount-1; j>=0; j-- )
		{
			FBFCurveKey lKey = lFCurve->Keys[j];
			// index = j
			// if comes sooner
			if ( (FBTime)lKey.Time < localTime )
			{
				// se value =/= zero ou value == 0 && include zero
				if ( lKey.Value != 0 || includeZero )
				{
					FBTime lDelta = localTime - (FBTime)lKey.Time;
					if ( lDelta < minDelta )
					{
						minDelta = lDelta;
						minIndex = i; // index do marker
						// j daria o index do key
						// sai do loop pq as próximas serão mais distantes
						break;
					}
				}
			}
		}
	}
	return minIndex;
}



int constraintcontactpoint::RemoveArrayItemAt( FBAnimationNode** pArray, int pIndex, int pLength)
{
	for (int i = pIndex; i < pLength-1; i++)
		pArray[i] = pArray[i + 1]; // copy next element left
	return pLength - 1;
}



void constraintcontactpoint::DeleteNodeAt( int pIndex )
{
	// defocus
	DefocusAll();
	DeselectAll();

	// pega o marker
	FBModel* lMarker = mMarkerList.GetAt( pIndex );
	
	// tira source
	mSourceList.RemoveAt( pIndex );
	// remove marker from list
	mMarkerList.RemoveAt( pIndex );
	mUINodeList.RemoveAt( pIndex );
	// tira os nodes
	RemoveArrayItemAt( mSourceNodes, pIndex, mMarkCount );
	RemoveArrayItemAt( mSourceMarkers, pIndex, mMarkCount );
	RemoveArrayItemAt( mMarkerWeights, pIndex, mMarkCount );
	
	//reduz a contagem
	mMarkCount--;

	// marca seleção nula
	mListSelIndex = -1;
	
	// apaga marker
	if ( lMarker )
		lMarker->FBDelete();
	
	
}


void constraintcontactpoint::ResetTool()
{
	//	defocus and deselect
	DefocusAll();
	DeselectAll();

	// stop constraint
	isMarkerConstrained = false;

	//register that no longer is set up
	SetSetUp(false);
	
	// clear selection information
	mListSelIndex = -1;

	// animation nodes - keep as is, just forget about them
	mMarkCount = 0;
	mSourceMarkers[0] = NULL;
	mSourceNodes[0] = NULL;
	mMarkerWeights[0] = NULL;
	
	// delete markers
	int lCnt = mMarkerList.GetCount();
	for( int i=0; i<lCnt; i++ )
		mMarkerList.GetAt( i )->FBDelete();
	
	// delete extra reference rig
	DeleteHierarchy( mCopyTopNode );
	mCopyTopNode = NULL;
	
	// clear lists
	mMarkerList.Clear();
	mSourceList.Clear();
	mUINodeList.Clear();

	// clear root information
	mRootTopNode = NULL;
	mRootString = FBString(); //FBString("");
	mRootNameSpace = FBString();
}



/* -- Development helper function
void constraintcontactpoint::DebugIntMessage( int pNumber )
{
	char name[50];
	sprintf(name,"Number is: %i", pNumber);
	FBMessageBox("Message", name, "OK", NULL, NULL, 1 );
}
*/