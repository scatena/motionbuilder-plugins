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
 *	Real-Time Engine Evaluation.
 ************************************************/
bool constraintcontactpoint::AnimationNodeNotify(FBAnimationNode* pConnector,FBEvaluateInfo* pEvaluateInfo,FBConstraintInfo* pConstraintInfo)
{
	// Only read and evaluate data if internally activated
	if (isMarkerConstrained)
	{
		// Intanced new vector for Target position
		double lDestPos[3];
		// Auxiliary double - reading weight of markers
		double total_weight;
		total_weight = 0;
		// Iterate over markers and sum weight values
		for( int i=0; i<mMarkCount; i++)
		{
			double lWeight[1];
			mMarkerWeights[i]->ReadData	( lWeight, pEvaluateInfo );
			total_weight += lWeight[0];
		}
		// Make it minimum '100'
		if ( total_weight < 100 )
			total_weight = 100.0;
		// Read Hips Position
		double lHipsPos[3];
		if( mSourceHip )
		{
			mSourceHip->ReadData		( lHipsPos, pEvaluateInfo );
			// Set Target to follow Source
			for( int j=0; j<3; j++ )
			{
				lDestPos[j] = lHipsPos[j];
			}
		}
		// Iterate over markers
		for( int i=0; i<mMarkCount; i++ )
		{
			double lMarkPos[3];
			double lNodePos[3];
			double lWeight[1];

			// Safety check (ultimately unnecessary)
			if( mSourceNodes[i] && mSourceMarkers[i] && mMarkerWeights[i] )
			{
				// Read data from source nodes, markers and its weight
				mSourceNodes[i]->ReadData	( lNodePos, pEvaluateInfo );
				mSourceMarkers[i]->ReadData	( lMarkPos, pEvaluateInfo );
				mMarkerWeights[i]->ReadData	( lWeight, pEvaluateInfo );

				// Set Target to be adjusted accordingly
				for( int j=0; j<3; j++)
				{
					lDestPos[j] += lWeight[0]/total_weight * ( lMarkPos[j] - lNodePos[j] );
				}
			}
		}
		// Write translation data do destination node
		if ( mDestinHip )
		{
			mDestinHip->WriteData	( lDestPos, pEvaluateInfo );
		}
	}
	return true;
}


/************************************************
 *	FBX Storage.
 ************************************************/
#define CONSTRAINT_FBX_VERSION		"Version"
#define	CONSTRAINT_FBX_PROPERTIES	"Properties"
#define CONSTRAINT_FBX_MARKERLIST	"MarkerList"
#define CONSTRAINT_FBX_SOURCELIST	"SourceList"
#define CONSTRAINT_FBX_UINODELIST	"UINodeList"
#define CONSTRAINT_FBX_SETTINGS		"Settings"
bool constraintcontactpoint::FbxStore( FBFbxObject* pFbxObject, kFbxObjectStore pStoreWhat )
{
	// Storing data for the tool to be persistent
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
	// Using FBXRetrieve to allow for constraint reconstruction
	if (pStoreWhat & kAttributes) // Avoid multiple readings
	{
		int	lVersion = 0;

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


//   ########################
//   ########################
//		 Public Functions
//   ########################
//   ########################


// Set the Root node
void constraintcontactpoint::SetRootNode( FBModel* pRootModel )
{
	mRootNode = pRootModel;
	SetRootString( (FBString)pRootModel->LongName, (FBString)pRootModel->Name );
}


// Set the Root string and its namespace
void constraintcontactpoint::SetRootString( FBString pLongName, FBString pShortName )
{	
	// Set root string
	mRootString = pLongName;

	// Set namespace - get length of short name and long name
	int lNameLength = pShortName.GetLen();
	int lFullNameLength = pLongName.GetLen();
	// Get everything to the left of the full name (meaning "full name" minus "short name")
	mRootNameSpace = pLongName.Left( lFullNameLength - lNameLength );
}


// Get the Root string
FBString constraintcontactpoint::GetRootString()
{	
	return mRootString;
}


// Add Node to the Nodes list
int constraintcontactpoint::AddComponentNode( FBComponent* pComponent )
{
	// Safety check
	if ( !pComponent )
		return 0;
	
	// If node is skeleton
	if( pComponent->Is( FBModelSkeleton::TypeInfo ) )
	{
		// Check if already setup
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
					
						// Register the addition
						return 1;
					}
				}
				// Else, parallel model was not found - warn user
				FBMessageBox("Warning", "Could not find equivalent node in parallel copy.\n(Probably due to naming conflicts)\nTry adding the bone directly from copied hierarchy.", "OK", NULL, NULL, 1 );
			}
			else if ( lTopNode == mCopyTopNode ) // Dragged model is part of copy hierachy
			{
				// Register it - dragged node both as reference and as marker target
				if ( RegisterNode( (FBModel*) pComponent, (FBModel*) pComponent ) )
				{
					// Add item to nodes list
					mUINodeList.Add( (FBModel*) pComponent );
				
					// Register the addition
					return 1;
				}
			}
			// Else, dragged model has no relation to the hierarchy - ignore it
		}
	}
	return 0;
}


// Get Nodes List count
int	constraintcontactpoint::GetNodeCount()
{
	return mUINodeList.GetCount();
}


// Get Name of Node from Nodes List at Index
const char*	constraintcontactpoint::GetNodeName( int pIndex )
{
	return mUINodeList[pIndex]->LongName;
}


// Get Marker from MarkerList at Index
FBModel* constraintcontactpoint::GetMarkerAt( int pIndex )
{
	return mMarkerList.GetAt( pIndex );
}


// Show Marker from MarkerList at Index
void constraintcontactpoint::ShowMarker( int pIndex )
{
	mMarkerList.GetAt( pIndex )->Show = true;
}


// Select Marker from MarkerList at Index
void constraintcontactpoint::SelectMarker( int pIndex )
{
	if ( mMarkerList[pIndex] )
	{
		DeselectAll();
		mMarkerList[pIndex]->Selected = true;
	}
}


// Focus Marker's Property from MarkerList at Index
void constraintcontactpoint::FocusMarker( int pIndex )
{
	if ( mMarkerList[pIndex] )
	{
		DefocusAll();
		Focus( mMarkerList[pIndex] );
	}
}


// Auto Key Interface
void constraintcontactpoint::SetAutoKey( bool pBool )
{
	isAutoKey = pBool;
}
bool constraintcontactpoint::GetAutoKey()
{
	return isAutoKey;
}


// Auto Select Interface
void constraintcontactpoint::SetAutoSelect( bool pBool )
{
	isAutoSelect = pBool;
}
bool constraintcontactpoint::GetAutoSelect()
{
	return isAutoSelect;
}


// Scene Select Interface
void constraintcontactpoint::SetSceneSelect( bool pBool )
{
	isSceneSelect = pBool;
}
bool constraintcontactpoint::GetSceneSelect()
{
	return isSceneSelect;
}


// Persistent Selection Interface
void constraintcontactpoint::SetPersistentSelect( bool pBool )
{
	isPersistentSelect = pBool;
}
bool constraintcontactpoint::GetPersistentSelect()
{
	return isPersistentSelect;
}


// Markers' Display Interface
void constraintcontactpoint::SetDisplayMarkerType( int pType )
{
	mDisplayMarkerType = pType;
}
int constraintcontactpoint::GetDisplayMarkerType()
{
	return mDisplayMarkerType;
}


// Keyframe Navigation Option Interface
void constraintcontactpoint::SetFindAllKeyframes( bool pBool )
{
	isFindAllKeyframes = pBool;
}
bool constraintcontactpoint::GetFindAllKeyframes()
{
	return isFindAllKeyframes;
}


// Persistent Selection Information Interface
void constraintcontactpoint::SetListSelIndex( int pIndex )
{
	mListSelIndex = pIndex;
}
int constraintcontactpoint::GetListSelIndex()
{
	return mListSelIndex;
}


// Setup Status Interface
bool constraintcontactpoint::GetSetUp ()
{
	return isSetUp;
}


// Reset weight properties focus
void constraintcontactpoint::DefocusAll()
{
	// Iterate and defocus
	for( int i=0; i<mMarkCount && mMarkCount == mMarkerList.GetCount(); i++)
	{
		FBModel* lMarker = mMarkerList.GetAt(i);
		if ( lMarker->PropertyList.Find("Weight") )
		{
			( (FBPropertyAnimatable*)lMarker->PropertyList.Find("Weight") )->SetFocus(false);
		}
	}
}


// Deselect All Components
void constraintcontactpoint::DeselectAll()
{
	// Iterate over Scene Components
	FBPropertyListComponent lList = FBSystem().Scene->Components;
	int lCnt = lList.GetCount();
	for( int i=0; i<lCnt; i++ )
	{
		FBComponent* lComp = lList.GetAt(i);
		if ( lComp->Selected )
			lComp->Selected = false;
	}
}


// Keyframe Navigation - Find Node with Currently Highest Weight Property
int constraintcontactpoint::FindWeight()
{
	int maxWeight = 0;
	int maxIndex = -1;
	// Iterate over markers
	for( int i=0; i<mMarkCount; i++ )
	{
		// Get value via Read Property
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


// Keyframe Navigation - Find Node with Nearest Next Weight Keyframe
int constraintcontactpoint::FindNextKey()
{
	bool includeZero = isFindAllKeyframes;
	FBTime localTime = FBSystem().LocalTime;
	FBTime minDelta = FBTime().OneHour;
	int minIndex = -1;
	// Iterate over markers
	for( int i=0; i<mMarkCount; i++ )
	{
		// Get value via FCurve
		FBAnimationNode* lNode = mMarkerWeights[i];
		FBFCurve* lFCurve = lNode->FCurve;
		// Iterate over Keys
		int lCount = lFCurve->Keys.GetCount();
		for( int j=0; j<lCount; j++ )
		{
			FBFCurveKey lKey = lFCurve->Keys[j];
			// If Key is set after current time
			if ( (FBTime)lKey.Time > localTime )
			{
				// If value is not zero or value is zero but tool set to Include Zero
				if ( lKey.Value != 0 || includeZero )
				{
					// Read time difference
					FBTime lDelta = (FBTime)lKey.Time - localTime;
					if ( lDelta < minDelta )
					{
						minDelta = lDelta;
						minIndex = i; // i = Marker Index ( j = Key Index; not to be confused!)
						// Leave loop as next keys can not be nearer (only further away in time)
						break;
					}
				}
			}
		}
	}
	return minIndex;
}


// Keyframe Navigation - Find Node with Nearest Next Weight Keyframe
int constraintcontactpoint::FindPrevKey()
{
	bool includeZero = isFindAllKeyframes;
	FBTime localTime = FBSystem().LocalTime;
	FBTime minDelta = FBTime().OneHour;
	int minIndex = -1;
	// Iterate over markers
	for( int i=0; i<mMarkCount; i++ )
	{
		// Get value via FCurve
		FBAnimationNode* lNode = mMarkerWeights[i];
		FBFCurve* lFCurve = lNode->FCurve;
		// Iterate over Keys - Reversed order (last to first)
		int lCount = lFCurve->Keys.GetCount();
		for( int j=lCount-1; j>=0; j-- )
		{
			FBFCurveKey lKey = lFCurve->Keys[j];
			// If Key is set before current time
			if ( (FBTime)lKey.Time < localTime )
			{
				// If value is not zero or value is zero but tool set to Include Zero
				if ( lKey.Value != 0 || includeZero )
				{
					// Read time difference
					FBTime lDelta = localTime - (FBTime)lKey.Time;
					if ( lDelta < minDelta )
					{
						minDelta = lDelta;
						minIndex = i; // i = Marker Index ( j = Key Index; not to be confused!)
						// Leave loop as next keys can not be nearer (only further away in time)
						break;
					}
				}
			}
		}
	}
	return minIndex;
}


// Set Visibility for All Markers
void constraintcontactpoint::SetMarkersVisibility( bool pBool )
{
	int lCnt = mMarkerList.GetCount();
	// Iterate over markers
	for( int i=0; i<lCnt; i++ )
	{
		mMarkerList.GetAt( i )->Show = pBool;
	}
}


// Setup the auxiliary rig (a reference copy of current hierarchy)
bool constraintcontactpoint::SetUpAuxRig ()
{
	// Deselect everything
	DeselectAll();
	// Get the 'Root' node
	if ( mRootNode )
	{
		// Get current time
		FBPlayerControl mPlayerControl; 
		FBTime localTime = FBSystem().LocalTime;

		// Store the top of original hierarchy
		mRootTopNode = GetTopParent( mRootNode );

		// Select branches below it (children)
		SelectSkeletonHierarchy( mRootNode );

		// Select hierarchy above (parents)
		SelectParentChain( mRootNode );

		// Duplicate via Save Selection - leave in case of error
		if ( !FileTempSaveSelect() )
			return false;

		// Register Setup (UI Refresh requirement)
		SetSetUp ( true );

		// Import duplicate copy - leave in case of error
		if ( !FileMergeBack() )
		{
			SetSetUp ( false );
			return false;
		}

		// Return to current time ("merge" will go to take start)
		mPlayerControl.Goto(localTime);
		 
		// Find copied node
		mCopyNode = FBFindModelByLabelName( COPY_NAMESPACE_DOT + mRootString );
		if ( mCopyNode )
		{
			// Colorize hierarchy
			ColorSkeletonBranch( (FBModelSkeleton*)mCopyNode );
			// Store the top of copied hierarchy
			mCopyTopNode = GetTopParent( mCopyNode );
		}
		
		// Activate constraint
		this->Active = true;

		return true;
	}
	// In case something went wrong
	SetSetUp ( false );
	return false;
}


// Delete marker and its node references at index
void constraintcontactpoint::DeleteNodeAt( int pIndex )
{
	// Reset focus and selection
	DefocusAll();
	DeselectAll();

	// Get the marker
	FBModel* lMarker = mMarkerList.GetAt( pIndex );
	
	// Remove reference on source list
	mSourceList.RemoveAt( pIndex );
	// Remove reference on marker list
	mMarkerList.RemoveAt( pIndex );
	// Remove reference on nodes list
	mUINodeList.RemoveAt( pIndex );
	// Remove references on Animation Nodes
	RemoveArrayItemAt( mSourceNodes, pIndex, mMarkCount );
	RemoveArrayItemAt( mSourceMarkers, pIndex, mMarkCount );
	RemoveArrayItemAt( mMarkerWeights, pIndex, mMarkCount );
	
	// Count it off
	mMarkCount--;

	// Clear UI selection
	mListSelIndex = -1;
	
	// Delete the marker from scene
	if ( lMarker )
		lMarker->FBDelete();	
}


// Reset Constraint
void constraintcontactpoint::ResetTool()
{
	// Reset focus and selection
	DefocusAll();
	DeselectAll();

	// Stop constraint
	isMarkerConstrained = false;

	// Register that no longer it is set up
	SetSetUp(false);
	
	// Clear UI selection
	mListSelIndex = -1;

	// Zero out count
	mMarkCount = 0;
	// Animation nodes - forget its header
	mSourceMarkers[0] = NULL;
	mSourceNodes[0] = NULL;
	mMarkerWeights[0] = NULL;
	
	// Iterate through Markers and delete them
	int lCnt = mMarkerList.GetCount();
	for( int i=0; i<lCnt; i++ )
		mMarkerList.GetAt( i )->FBDelete();
	
	// Delete copied Reference Rig
	DeleteHierarchy( mCopyTopNode );
	mCopyTopNode = NULL;
	
	// Clear Lists
	mMarkerList.Clear();
	mSourceList.Clear();
	mUINodeList.Clear();

	// Clear Root Node information
	mRootTopNode = NULL;
	mRootString = FBString();
	mRootNameSpace = FBString();
}


//   #########################
//   #########################
//		 Private Functions
//   #########################
//   #########################


// Save selection to a temporary file
bool constraintcontactpoint::FileTempSaveSelect()
{
	// Save options
	FBFbxOptions lOptions = FBFbxOptions(false);
	lOptions.SaveSelectedModelsOnly = true;
    lOptions.BaseCameras = false;
    lOptions.CameraSwitcherSettings = false;
    lOptions.CurrentCameraSettings = false;
    lOptions.GlobalLightingSettings = false;
    lOptions.TransportSettings = false;

	// Temporary path (using the system TempPath - usually AppData)
	mTempSaveString = FBString(mSystem.TempPath) + "SaveSelected.fbx";
	
	// Save selected models into a temp file
	bool lResult = mApplication.FileSave( mTempSaveString, &lOptions );

	// Warn in case of error
	if( !lResult )
		FBMessageBox("Error", "Could not duplicate structure.\nExport error.", "OK", NULL, NULL, 1 );

	return lResult;
}


// Merge temporary file back into the scene
bool constraintcontactpoint::FileMergeBack()
{
	// Import options
	FBFbxOptions lOptions = FBFbxOptions(false);
	lOptions.NamespaceList = COPY_NAMESPACE;

	// merge from temp file
	bool lResult = mApplication.FileMerge ( mTempSaveString, false, &lOptions);

	// Warn in case of error
	if( !lResult )
		FBMessageBox("Error", "Could not duplicate structure.\nImport error.", "OK", NULL, NULL, 1 );

	return lResult;
}


// Get the top parent of a model
FBModel* constraintcontactpoint::GetTopParent( FBModel* pModel )
{
	if ( pModel )
	{
		// Recursion
		if ( pModel->Parent )
			return GetTopParent( pModel->Parent );
		else
			return pModel;
	}
	// Case none, return none
	return NULL;
}


// Get the top namespace of a model
FBNamespace* constraintcontactpoint::GetParentNamespace( FBNamespace* pName )
{
	if ( pName )
	{
		// Recursion
		if ( pName->GetOwnerNamespace() )
			return GetParentNamespace( pName->GetOwnerNamespace() );
		else
			return pName;
	}
	// Case none, return none
	return NULL;
}


// Get the constraint source root model
FBModel* constraintcontactpoint::FindRootSrc()
{
	// Use the root of copied hierarchy as source
	return mCopyNode;
}


// Get the constraint destination root model
FBModel* constraintcontactpoint::FindRootDst()
{
	// Use the root of source hierarchy as destination
	return mRootNode;
}


// Register set up status
void constraintcontactpoint::SetSetUp ( bool pActivate )
{
	isSetUp = pActivate;
}


// Register the model added to list (from UI)
bool constraintcontactpoint::RegisterNode( FBModel* pLocalModel, FBModel* pTargetModel )
{
	// Create a marker - and configure if successfully created
	if ( NewLinkedMarker( pTargetModel ) )
	{
		// Append to list
		mSourceList.Add( pLocalModel );

		// Set up animation node
		SetupAnimationNode();

		// Status check - If set to 'Auto Key', set key
		if ( isAutoKey )
			AutoKey();
		
		// Status check - If set to 'Display All', display marker
		if ( mDisplayMarkerType == DISPLAYALL )
		{
			mMarkerList[mMarkCount-1]->Show = true;
		}
		
		// Status check - If set to 'Auto Select', select it (in UI list)
		if ( isAutoSelect )
		{
			// Register selection index
			SetListSelIndex( mMarkCount - 1 );
			// Status check - If set to 'Scene Select', select it (in scene)
			if ( isSceneSelect )
			{
				// Deselect others, select current
				DeselectAll();
				mMarkerList[mMarkCount-1]->Selected = true;
			}
			// Status check - If set to 'Display Selected', display only current marker
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


// Create and configure a new marker linked to a reference model (auxiliary to RegisterNode)
FBModel* constraintcontactpoint::NewLinkedMarker( FBModel* pParentModel )
{
	// Marker count limit
	if ( mMarkCount == 100 )
	{
		// Do not allow more
		return NULL;
	}

	// Name it - padded counter
	char paddednumc[10];
	sprintf( paddednumc, "%02d", mMarkCount ); 
	FBString pName = FBString("Mark") + FBString(paddednumc) + FBString("_") + pParentModel->Name ;

	// TODO - allow more than '99' in case user deletes marker and adds new ones

	// Create the marker
	FBModel* lMarker = CreateMarker( pName );
	
	// If it is the first one, create a parent node
	if ( mMarkCount == 1 )
		mMarkerParent = new FBModelNull("MarkersGrp");

	// Make the new marker a child of it
	lMarker->Parent = mMarkerParent;

	// Snap marker to model
	SnapModel( lMarker, pParentModel );

	// Add "Weight" Property
	FBPropertyAnimatable* lProp = (FBPropertyAnimatable*)lMarker->PropertyCreate("Weight", kFBPT_int, "Integer", true, true, NULL);

	// Set Minimum, Max and Clamp (alt. SetMinMax)
	lProp->SetMin(0, true);
	lProp->SetMax(100, true);
	// SetAnimated - exposing its property node
	lProp->SetAnimated(true);

	// Add to list
	mMarkerList.Add(lMarker);

	return lMarker;
}


// Create a new marker (auxiliary to NewLinkedMarker)
FBModel* constraintcontactpoint::CreateMarker(char* pName )
{
	// New Marker
	FBModel* lMarker = new FBModelMarker( pName );

	// Adjust its size
	double lSize = 300;
	FBProperty* lPproperty = lMarker->PropertyList.Find("Size");
	lPproperty->SetData(&lSize);
	
	// Adjust its look (light cross - 2)
	int lLook = kFBMarkerLookLightCross;
	lPproperty = lMarker->PropertyList.Find("Look");
	lPproperty->SetData(&lLook);
	
	// Increment the count
	mMarkCount += 1;

	return lMarker;
}


// Snaps one model to the position of the other (auxiliary to NewLinkedMarker)
void constraintcontactpoint::SnapModel( FBModel* pDst, FBModel* pSrc )
{
	FBVector3d l3dGlobalTrans;
	// 'True' flag for global
	pSrc->GetVector(l3dGlobalTrans, kModelTranslation, true ); 
    pDst->Translation = l3dGlobalTrans;
}


// Create and set up keys for created marker at the moment (auxiliary to RegisterNode)
void constraintcontactpoint::AutoKey()
{
	// Getting the weight node...
	if ( mMarkCount > 0 )
	{
		// ...via created node
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
			}
		}
	}
}


// Create zero-weight keys on any non-zero weighted marker at the moment (auxiliary to AutoKey)
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
			// Check if weight is non-zero
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


// Focus markers's weight property
void constraintcontactpoint::Focus( FBModel* pMarker )
{
	if ( pMarker->PropertyList.Find("Weight") )
	{
		( (FBPropertyAnimatable*)pMarker->PropertyList.Find("Weight") )->SetFocus(true);
	}
}


// Set up animation nodes for real-time evaluation (progressively added)
void constraintcontactpoint::SetupAnimationNode()
{
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
		// Source model node
		FBModel* srcNode = mSourceList.GetAt( mMarkCount-1 );
		if ( srcNode )
		{
			mSourceNodes[mMarkCount-1]	= AnimationNodeOutCreate( 2, srcNode, ANIMATIONNODE_TYPE_TRANSLATION );
		}
		else
			FBMessageBox("Tool error", "Reference node was not found", "OK", NULL, NULL, 1 );
		// Marker node
		FBModel* srcMark = mMarkerList.GetAt( mMarkCount-1 );
		if ( srcMark )
		{
			//  Its position...
			mSourceMarkers[mMarkCount-1] = AnimationNodeOutCreate ( 3, srcMark, ANIMATIONNODE_TYPE_TRANSLATION );
			// ...plus its weight property value
			mMarkerWeights[mMarkCount-1] = AnimationNodeOutCreate ( 4, srcMark, ANIMATIONNODE_TYPE_INTEGER );
		}
		else
			FBMessageBox("Tool error", "Reference marker was not found", "OK", NULL, NULL, 1 );
	}
}


// Set up animation nodes for real-time evaluation (one-shot creation for recreation from file open)
void constraintcontactpoint::ReSetupAnimationNode()
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

	// Iterate through markers
	for( int i=0; i<mMarkCount; i++ )
	{
		// Source model node
		FBModel* srcNode = mSourceList.GetAt(i);
		if ( srcNode )
		{
			mSourceNodes[i]	= AnimationNodeOutCreate( 2, srcNode, ANIMATIONNODE_TYPE_TRANSLATION );
		}
		else
			FBMessageBox("Tool error", "Reference node was not found", "OK", NULL, NULL, 1 );
		// Marker node
		FBModel* srcMark = mMarkerList.GetAt( i );
		if ( srcMark )
		{
			//  Its position...
			mSourceMarkers[i] = AnimationNodeOutCreate ( 3, srcMark, ANIMATIONNODE_TYPE_TRANSLATION );
			// ...plus its weight property value
			mMarkerWeights[i] = AnimationNodeOutCreate ( 4, srcMark, ANIMATIONNODE_TYPE_INTEGER );
		}
		else
			FBMessageBox("Tool error", "Reference marker was not found", "OK", NULL, NULL, 1 );
	}
}


// Delete item from nodes array at Index
int constraintcontactpoint::RemoveArrayItemAt( FBAnimationNode** pArray, int pIndex, int pLength)
{
	// Deleting: overwriting removed value, shifting all items after it to the left
	// and registering the array to be one item shorter
	for (int i = pIndex; i < pLength-1; i++)
		pArray[i] = pArray[i + 1]; // Copy next element to the left
	return pLength - 1;
}


// Select model and full hierarchy - recursive
void constraintcontactpoint::SelectSkeletonHierarchy( FBModel* topNode )
{
	// Safety check - exit if is not skeleton
	if ( topNode == NULL || !topNode->Is( FBModelSkeleton::TypeInfo ) )
		return;
	
	// Get number of children
	int lCnt = topNode->Children.GetCount();
	// Recursion
	for( int i=0; i<lCnt; i++ )
	{
		SelectSkeletonHierarchy( (FBModel*)topNode->Children.GetAt(i) );
	}
	// Select it
	topNode->Selected = true;
}


// Select all parents of a model - recursive
void constraintcontactpoint::SelectParentChain( FBModel* pModel )
{
	// Safety check
	if ( pModel == NULL )
		return;

	// If there is parent
	if ( pModel->Parent )
	{
		SelectParentChain( pModel->Parent );
		pModel->Parent->Selected = true;
	}
}


// Colorize full skeleton hierarchy - recursive
void constraintcontactpoint::ColorSkeletonBranch( FBModelSkeleton* topNode )
{
	// Safety check - exit if is not skeleton
	if ( topNode == NULL || !topNode->Is( FBModelSkeleton::TypeInfo ) )
		return;
	
	// Get number of children
	int lCnt = topNode->Children.GetCount();
	// Recursion
	for( int i=0; i<lCnt; i++ )
	{
		ColorSkeletonBranch( (FBModelSkeleton*)topNode->Children.GetAt(i) );
	}
	// Colorize it
	topNode->Color = FBColor(0,0.666,1);
}


// Delete a model and its full hierarchy - recursive
void constraintcontactpoint::DeleteHierarchy( FBModel* topNode )
{
	// Safety check
	if ( topNode == NULL )
		return;
	
	// Delete every children back to front
	// Get number of children
	int lCnt = topNode->Children.GetCount() - 1;
	// Recursion
	for( int i=0; i<=lCnt; i++ )
	{
		DeleteHierarchy( (FBModel*)topNode->Children.GetAt( lCnt-i ) );
	}
	// Delete it
	topNode->FBDelete();
}


/* -- Development helper function
void constraintcontactpoint::DebugIntMessage( int pNumber )
{
	char name[50];
	sprintf(name,"Number is: %i", pNumber);
	FBMessageBox("Message", name, "OK", NULL, NULL, 1 );
}
*/