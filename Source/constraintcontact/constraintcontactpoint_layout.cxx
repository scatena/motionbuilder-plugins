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

/**	\file	constraintcontactpoint_layout.cxx
*	Definition of a layout class for a simple constraint.
*	Simple constraint layout class, constraintcontactpoint_Layout for
*	the simple constraint example.
*/

//--- Class declaration
#include "constraintcontactpoint_layout.h"

//--- Registration define
#define ORCONSTRAINT__LAYOUT	constraintcontactpoint_Layout

//--- implementation and registration
FBConstraintLayoutImplementation(	ORCONSTRAINT__LAYOUT		);		// Layout class name
FBRegisterConstraintLayout		(	ORCONSTRAINT__LAYOUT,				// Layout class name
									ORCONSTRAINT__CLASSSTR,				// Constraint class name (for association)
									FB_DEFAULT_SDK_ICON			);		// Icon filename (default=Open Reality icon)



/************************************************
 *	Creation function..
 ************************************************/
bool constraintcontactpoint_Layout::FBCreate()
{
	// Assign handle onto constraint (cast generic pointer)
	mConstraint	= (constraintcontactpoint*) (FBConstraint*) Constraint;

	// Create & configure the UI
	UICreate	();		// create the UI
	UIConfigure	();		// configure the UI
	UIReset		();		// set the UI values from the constraint

	return true;
}


/************************************************
 *	Destructor for layout.
 ************************************************/
void constraintcontactpoint_Layout::FBDestroy()
{
	// Defocus if is not on persistent selection
	if ( !mConstraint->GetPersistentSelect() )
		mConstraint->DefocusAll();
}


/************************************************
 *	UI Creation function.
 *	Create UI regions and assign them to UI elements
 ************************************************/
void constraintcontactpoint_Layout::UICreate()
{
	int lS = 4;
	int lH = 18;

	// Create UI
	AddRegion(	"TitleLabel",		"TitleLabel",										// Title Row
											 6,	kFBAttachLeft,	"",			1.0,	//x
											lS,	kFBAttachTop,	"",			1.0,	//y
											198,kFBAttachNone,	NULL,		1.0,	//w
											25,	kFBAttachNone,	NULL,		1.0 );	//h

	AddRegion(	"ButtonConfig",		"ButtonConfig",
											lS, kFBAttachRight,	"TitleLabel",1.0,
											 9,	kFBAttachTop,	"",			1.0,
											45,	kFBAttachNone,	NULL,		1.0,
											lH,	kFBAttachNone,	NULL,		1.0 );

	AddRegion(	"ButtonAbout",		"ButtonAbout",
											 2, kFBAttachRight,	"ButtonConfig",	1.0,
											 9,	kFBAttachTop,	"",			1.0,
											12,	kFBAttachNone,	NULL,		1.0,
											lH,	kFBAttachNone,	NULL,		1.0 );
	AddRegion(	"ListRoot",			"ListRoot",											// Root and Setup Row		
                                            lS, kFBAttachLeft,  "",			1.0,
                                            lS,	kFBAttachBottom,"TitleLabel",	1.0,
                                            198,kFBAttachNone,	NULL,		1.0,
                                            lH, kFBAttachNone,  NULL,		1.0 );
	AddRegion(	"ButtonSetup",		"ButtonSetup",
											lS, kFBAttachRight,	"ListRoot",	1.0,
											0,  kFBAttachTop,	"ListRoot", 1.0,
											58,	kFBAttachNone,	NULL,		1.0,
											lH,	kFBAttachNone,	NULL,		1.0 );
	AddRegion(	"ListModel",		"ListModel",										// Model Row
                                            lS, kFBAttachLeft,  "",			1.0,
                                            lS, kFBAttachBottom,"ListRoot", 1.0,
                                            260,kFBAttachNone,	NULL,		1.0,
                                            145,kFBAttachNone,	NULL,		1.0 );
    AddRegion(	"ButtonPrevNode",	"ButtonPrevNode",									// Navigation and Property Row
                                            lS, kFBAttachLeft,  "",			1.0,
                                            lS,	kFBAttachBottom,"ListModel",1.0,
                                            12,	kFBAttachNone,	NULL,		1.0,
                                            15,	kFBAttachNone,	NULL,		1.0 );
    AddRegion(	"ButtonFindNode",	"ButtonFindNode",
                                             2, kFBAttachRight,	"ButtonPrevNode",1.0,
                                            lS,	kFBAttachBottom,"ListModel",1.0,
                                            12,	kFBAttachNone,	NULL,		1.0,
                                            15,	kFBAttachNone,	NULL,		1.0 );
	AddRegion( "ButtonNextNode",	"ButtonNextNode",
		                                     2, kFBAttachRight,	"ButtonFindNode",1.0,
                                            lS,	kFBAttachBottom,"ListModel",1.0,
                                            12,	kFBAttachNone,	NULL,		1.0,
                                            15,	kFBAttachNone,	NULL,		1.0 );
	AddRegion(	"MarkerProperty",	"MarkerProperty",
                                            lS, kFBAttachRight, "ButtonNextNode",1.0,
                                            lS,	kFBAttachBottom,"ListModel",1.0,
                                            193,kFBAttachNone,	NULL,		1.0,
                                            15, kFBAttachNone,  NULL,		1.0 );
	AddRegion(	"ButtonDelNode",	"ButtonDelNode",
                                            lS, kFBAttachRight,	"MarkerProperty",1.0,
                                            lS,	kFBAttachBottom,"ListModel",1.0,
                                            21, kFBAttachNone,	NULL,		1.0,
                                            15, kFBAttachNone,  NULL,		1.0 );


	// Assign regions
	SetControl( "TitleLabel",		mTitleLabel );
	SetControl( "ButtonAbout",		mButtonAbout );
	SetControl( "ButtonConfig",		mButtonConfig );
	SetControl( "ListRoot",			mListRoot );
	SetControl( "ButtonSetup",		mButtonSetup );
	SetControl( "ListModel",		mListModel );
	SetControl( "ButtonDelNode",	mButtonDelNode );
    SetControl( "MarkerProperty",	mMarkerProperty );
    SetControl( "ButtonPrevNode",	mButtonPrevNode );
	SetControl( "ButtonFindNode",	mButtonFindNode );
    SetControl( "ButtonNextNode",	mButtonNextNode );


	UICreatePopupConfig();
}

/************************************************
 *	Create & Assign the UI regions (for Popup).
 ************************************************/
void constraintcontactpoint_Layout::UICreatePopupConfig()
{
	int lS = 6;
	int lH = 18;

	// Popup size and placement
	mPopupConfig.Region.X		= 200;
	mPopupConfig.Region.Y		= 300;
	mPopupConfig.Region.Width	= 200;
	mPopupConfig.Region.Height= 260;

	// Add Regions - Pop-up - Config (Options)
	mPopupConfig.AddRegion("ButtonClosePopup",	"ButtonClosePopup",							// 'Close' Button
											-23,kFBAttachRight,	"",			1.0,
											lS,	kFBAttachTop,	"",			1.0,
											20, kFBAttachNone,	NULL,		1.0,
											lH, kFBAttachNone,  NULL,		1.0 );
	mPopupConfig.AddRegion("ButtonHoverHelp",		"ButtonHoverHelp",						// Hover Instructions
											-18,kFBAttachLeft,	"ButtonClosePopup",1.0,
											lS,	kFBAttachTop,	"",			1.0,
											13, kFBAttachNone,	NULL,		1.0,
											lH, kFBAttachNone,  NULL,		1.0 );
	mPopupConfig.AddRegion("PopupTitleLabel",		"PopupTitleLabel",							
											30,kFBAttachLeft,	"",			1.0,
											 9,	kFBAttachTop,	"",			1.0,
											110,kFBAttachNone,	NULL,		1.0,
											lH, kFBAttachNone,  NULL,		1.0 );
	mPopupConfig.AddRegion("PopupMarkersLabel",	"PopupMarkersLabel",
                                            10, kFBAttachLeft,  "",			1.0,
                                             8, kFBAttachBottom,"PopupTitleLabel",1.0,
                                            150,kFBAttachNone, NULL,		1.0,
                                            lH, kFBAttachNone,  NULL,		1.0 );
	mPopupConfig.AddRegion("ButtonAutoKeyCheck",	"ButtonAutoKeyCheck",					// Checkbox for Auto-key
                                            lS, kFBAttachLeft,	"", 1.0,
                                            lS, kFBAttachBottom,"PopupMarkersLabel",1.0,
                                            85,kFBAttachNone,	NULL,		1.0,
                                            lH, kFBAttachNone,  NULL,		1.0 );
	mPopupConfig.AddRegion("ButtonAutoSelectCheck",	"ButtonAutoSelectCheck",				// Checkbox for Auto-Select (UI)
                                            lS, kFBAttachRight, "ButtonAutoKeyCheck",1.0,
                                            lS, kFBAttachBottom,"PopupMarkersLabel",1.0,
                                            100,kFBAttachNone,	NULL,		1.0,
                                            lH, kFBAttachNone,  NULL,		1.0 );
	mPopupConfig.AddRegion("PopupSelectLabel",	"PopupSelectLabel",									
											10,	kFBAttachLeft,	"",			1.0,
											lS,	kFBAttachBottom,"ButtonAutoSelectCheck",1.0,
											150,kFBAttachNone,	NULL,		1.0,
											lH,	kFBAttachNone,	NULL,		1.0 );
	mPopupConfig.AddRegion("ButtonSceneSelectCheck",	"ButtonSceneSelectCheck",			// Checkbox for Auto-Select (Scene)
                                            lS, kFBAttachLeft,	"", 1.0,
                                            lS, kFBAttachBottom,"PopupSelectLabel", 1.0,
                                            85,kFBAttachNone,	NULL,		1.0,
                                            lH, kFBAttachNone,  NULL,		1.0 );
	mPopupConfig.AddRegion("ButtonPersistentSelectCheck",	"ButtonPersistentSelectCheck",	// Checkbox for Persistent Selection
                                            lS, kFBAttachRight, "ButtonSceneSelectCheck",1.0,
                                            lS, kFBAttachBottom,"PopupSelectLabel",	1.0,
                                            100,kFBAttachNone,	NULL,		1.0,
                                            lH, kFBAttachNone,  NULL,		1.0 );
	mPopupConfig.AddRegion("PopupDisplayLabel",	"PopupDisplayLabel",
											10,	kFBAttachLeft,	"",			1.0,
											lS,	kFBAttachBottom,"ButtonPersistentSelectCheck",1.0,
											160,kFBAttachNone,	NULL,		1.0,
											lH,	kFBAttachNone,	NULL,		1.0 );
	mPopupConfig.AddRegion("ButtonRadioDispAll",	"ButtonRadioDispAll",					// Radio buttons for Marker Display
                                            lS, kFBAttachLeft, "",1.0,
                                            lS, kFBAttachBottom,"PopupDisplayLabel",1.0,
                                            52, kFBAttachNone,	NULL,		1.0,
                                            lH, kFBAttachNone,  NULL,		1.0 );
	mPopupConfig.AddRegion("ButtonRadioDispSel",	"ButtonRadioDispSel",		
                                            lS, kFBAttachRight, "ButtonRadioDispAll",1.0,
                                             0, kFBAttachTop,	"ButtonRadioDispAll",1.0,
                                            76, kFBAttachNone,	NULL,		1.0,
                                             0, kFBAttachHeight,"ButtonRadioDispAll",1.0 );
	mPopupConfig.AddRegion("ButtonRadioDispNil",	"ButtonRadioDispNil",		
                                            lS, kFBAttachRight, "ButtonRadioDispSel",1.0,
                                             0, kFBAttachTop,	"ButtonRadioDispAll",1.0,
                                             0, kFBAttachWidth,	"ButtonRadioDispAll",1.0,
                                             0, kFBAttachHeight,"ButtonRadioDispAll",1.0 );
	mPopupConfig.AddRegion("PopupKeyframeLabel",	"PopupKeyframeLabel",
											10,	kFBAttachLeft,	"",			1.0,
											lS,	kFBAttachBottom,"ButtonRadioDispAll",1.0,
											160,kFBAttachNone,	NULL,		1.0,
											lH,	kFBAttachNone,	NULL,		1.0 );
	mPopupConfig.AddRegion("ButtonRadioFindAll",	"ButtonRadioFindAll",					// Radio Buttons for Navigation Options
                                            lS, kFBAttachLeft, "",1.0,
                                            lS, kFBAttachBottom,"PopupKeyframeLabel",1.0,
                                            52, kFBAttachNone,	NULL,		1.0,
                                            lH, kFBAttachNone,  NULL,		1.0 );
	mPopupConfig.AddRegion("ButtonRadioFindNon",	"ButtonRadioFindNon",		
                                            lS, kFBAttachRight, "ButtonRadioFindAll",1.0,
                                             0, kFBAttachTop,	"ButtonRadioFindAll",1.0,
                                            76, kFBAttachNone,	NULL,		1.0,
                                             0, kFBAttachHeight,"ButtonRadioFindAll",1.0 );
	mPopupConfig.AddRegion("PopupResetLabel",		"PopupResetLabel",
											10,	kFBAttachLeft,	"",			1.0,
											12,	kFBAttachBottom,"ButtonRadioFindAll",1.0,
											40, kFBAttachNone,	NULL,		1.0,
											lH,	kFBAttachNone,	NULL,		1.0 );
	mPopupConfig.AddRegion("ButtonReset",			"ButtonReset",							// 'Reset Tool' Button
                                            lS, kFBAttachRight, "PopupResetLabel",1.0,
                                            12, kFBAttachBottom,"ButtonRadioFindAll",1.0,
                                            80, kFBAttachNone,	NULL,		1.0,
                                            lH, kFBAttachNone,  NULL,		1.0 );


	// Assign regions - Pop-up - Config (Options)
	mPopupConfig.SetControl( "ButtonClosePopup",		mButtonCloseConfig );
	mPopupConfig.SetControl( "ButtonHoverHelp",			mButtonHoverHelp );
	mPopupConfig.SetControl( "PopupTitleLabel",			mPopupTitleLabel );

	mPopupConfig.SetControl( "PopupMarkersLabel",		mPopupMarkersLabel );
	mPopupConfig.SetControl( "ButtonAutoKeyCheck",		mButtonAutoKeyCheck );
	mPopupConfig.SetControl( "ButtonAutoSelectCheck",	mButtonAutoSelectCheck );

	mPopupConfig.SetControl( "PopupSelectLabel",		mPopupSelectLabel );
	mPopupConfig.SetControl( "ButtonSceneSelectCheck",	mButtonSceneSelectCheck );
	mPopupConfig.SetControl( "ButtonPersistentSelectCheck",mButtonPersistentSelectCheck );
	
	mPopupConfig.SetControl( "PopupDisplayLabel",		mPopupDisplayLabel );
	mPopupConfig.SetControl( "ButtonRadioDispAll",		mButtonRadioDispAll );
	mPopupConfig.SetControl( "ButtonRadioDispSel",		mButtonRadioDispSel );
	mPopupConfig.SetControl( "ButtonRadioDispNil",		mButtonRadioDispNil );

	mPopupConfig.SetControl( "PopupKeyframeLabel",		mPopupKeyframeLabel );
	mPopupConfig.SetControl( "ButtonRadioFindAll",		mButtonRadioFindAll );
	mPopupConfig.SetControl( "ButtonRadioFindNon",		mButtonRadioFindNon );

	mPopupConfig.SetControl( "PopupResetLabel",			mPopupResetLabel );
	mPopupConfig.SetControl( "ButtonReset",				mButtonReset );

	
	UICreatePopupAbout();
}



/************************************************
 *	Create & Assign the UI regions (for Popup).
 ************************************************/
void constraintcontactpoint_Layout::UICreatePopupAbout()
{
	// Popup size and placement
	mPopupAbout.Region.X		= 200;
	mPopupAbout.Region.Y		= 300;
	mPopupAbout.Region.Width	= 200;
	mPopupAbout.Region.Height= 260;

	// Add regions - Pop-up - About (Info)
	mPopupAbout.AddRegion("ButtonCloseInfo",		"ButtonCloseInfo",							// 'Close' Button
											-23,kFBAttachRight,	"",			1.0,
											 6,	kFBAttachTop,	"",			1.0,
											20, kFBAttachNone,	NULL,		1.0,
											18, kFBAttachNone,  NULL,		1.0 );
	mPopupAbout.AddRegion("PopupInfo1Label",		"PopupInfo1Label",							// Main Title						
											10,kFBAttachLeft,	"",1.0,
											 9,	kFBAttachTop,	"",			1.0,
											150,kFBAttachNone,	NULL,		1.0,
											30, kFBAttachNone,  NULL,		1.0 );
	mPopupAbout.AddRegion("PopupInfo2Label",		"PopupInfo2Label",							// Instructions Title
											12,kFBAttachLeft,	"",			1.0,
											 6,	kFBAttachBottom,"PopupInfo1Label",			1.0,
											150,kFBAttachNone,	NULL,		1.0,
											18, kFBAttachNone,  NULL,		1.0 );
	mPopupAbout.AddRegion("PopupInfo3Label",		"PopupInfo3Label",							// Instructions Body
                                            10, kFBAttachLeft,  "",			1.0,
                                             4, kFBAttachBottom,"PopupInfo2Label",1.0,
                                            180,kFBAttachNone, NULL,		1.0,
                                            92, kFBAttachNone,  NULL,		1.0 );
	mPopupAbout.AddRegion("PopupInfoLinkText",	"PopupInfoLinkText",							// Instructions Link
                                            10, kFBAttachLeft,	"", 1.0,
                                             4, kFBAttachBottom,"PopupInfo3Label",1.0,
                                            160,kFBAttachNone,	NULL,		1.0,
                                            18, kFBAttachNone,  NULL,		1.0 );
	mPopupAbout.AddRegion("PopupInfo4Label",		"PopupInfo4Label",							// Author Title
                                            12, kFBAttachLeft,	"", 1.0,
                                            10, kFBAttachBottom,"PopupInfoLinkText",1.0,
                                            160,kFBAttachNone,	NULL,		1.0,
                                            18, kFBAttachNone,  NULL,		1.0 );
	mPopupAbout.AddRegion("PopupInfo5Label",		"PopupInfo5Label",							// Author Info
											10,	kFBAttachLeft,	"",			1.0,
											 4,	kFBAttachBottom,"PopupInfo4Label",1.0,
											160,kFBAttachNone,	NULL,		1.0,
											40,	kFBAttachNone,	NULL,		1.0 );
	

	// Assign regions - Pop-up - About (Info)
	mPopupAbout.SetControl( "ButtonCloseInfo",            mButtonCloseAbout );
    mPopupAbout.SetControl( "PopupInfo1Label",            mPopupInfo1Label );
    mPopupAbout.SetControl( "PopupInfo2Label",            mPopupInfo2Label );
    mPopupAbout.SetControl( "PopupInfo3Label",            mPopupInfo3Label );
    mPopupAbout.SetControl( "PopupInfoLinkText",          mPopupInfoLinkText );
    mPopupAbout.SetControl( "PopupInfo4Label",            mPopupInfo4Label );
    mPopupAbout.SetControl( "PopupInfo5Label",            mPopupInfo5Label );
}


/************************************************
 *	UI Configuration function.
 *	Assign properties & callbacks for UI elements
 ************************************************/
void constraintcontactpoint_Layout::UIConfigure()
{
	// Title
	mTitleLabel.Caption =	"Contact Point Editing Tool";
	
	// Config Popup Button
	mButtonConfig.Caption = "Options";
	mButtonConfig.Justify = kFBTextJustifyRight;
	mButtonConfig.PropertyList.Find( "Hint" )->SetString( "Tool options" );
	mButtonConfig.OnClick.Add( this, (FBCallback) &constraintcontactpoint_Layout::EventButtonConfigPopupClick );

	// Info Popup Button
	mButtonAbout.Caption = "?";
	mButtonAbout.Justify = kFBTextJustifyRight;
	mButtonAbout.PropertyList.Find( "Hint" )->SetString( "About" );
	mButtonAbout.OnClick.Add( this, (FBCallback) &constraintcontactpoint_Layout::EventButtonAboutPopupClick );

	// Root List
	mListRoot.Style	= kFBVerticalList;
	mListRoot.PropertyList.Find( "Hint" )->SetString( "Add root of bone chain hierarchy" );
	mListRoot.OnDragAndDrop.Add( this, (FBCallback) &constraintcontactpoint_Layout::EventRootListDragAndDrop );
	mListRoot.OnChange.Add( this, (FBCallback) &constraintcontactpoint_Layout::EventRootListDeselect ); // Keep deselected

	// Setup button
	mButtonSetup.Caption = "Setup";
	mButtonSetup.PropertyList.Find( "Hint" )->SetString( "Set up tool" );
	mButtonSetup.OnClick.Add( this, (FBCallback) &constraintcontactpoint_Layout::EventButtonSetupClick );

	// Models List
	mListModel.Style = kFBVerticalList;
	mListModel.OnDragAndDrop.Add( this, (FBCallback) &constraintcontactpoint_Layout::EventModelListDragAndDrop );
	mListModel.OnChange.Add( this, (FBCallback) &constraintcontactpoint_Layout::EventModelListClick ); 

	// Keyframe Navigation Buttons
    mButtonPrevNode.Caption = "<";
	mButtonPrevNode.PropertyList.Find( "Hint" )->SetString( "Previous Node" );
	mButtonPrevNode.OnClick.Add( this, (FBCallback) &constraintcontactpoint_Layout::EventButtonPrevNodeClick );

    mButtonFindNode.Caption = "~";
	mButtonFindNode.PropertyList.Find( "Hint" )->SetString( "Currently active node" );
	mButtonFindNode.OnClick.Add( this, (FBCallback) &constraintcontactpoint_Layout::EventButtonFindNodeClick );

	mButtonNextNode.Caption = ">";
	mButtonNextNode.PropertyList.Find( "Hint" )->SetString( "Next Node" );
	mButtonNextNode.OnClick.Add( this, (FBCallback) &constraintcontactpoint_Layout::EventButtonNextNodeClick );

	// 'Delete' Button
	mButtonDelNode.Caption = "Del";
	mButtonDelNode.Justify = kFBTextJustifyRight;
	mButtonDelNode.PropertyList.Find( "Hint" )->SetString( "Remove selected node" );
	mButtonDelNode.OnClick.Add( this, (FBCallback) &constraintcontactpoint_Layout::EventButtonDelNodeClick );

	// List Selection Index (is used in config popup, thus initialized here)
	mSelectedNodeIndex = -1;

	UIConfigurePopupConfig();
}


/************************************************
 *	Configure & add callbacks to the UI Elements (for Popup).
 ************************************************/
void constraintcontactpoint_Layout::UIConfigurePopupConfig()
{
	// Popup Title
	mPopupConfig.Caption = "Options";

	// 'Close' Button
	mButtonCloseConfig.Caption = "X";
	mButtonCloseConfig.PropertyList.Find( "Hint" )->SetString( "Close window" );
	mButtonCloseConfig.OnClick.Add	( this, (FBCallback) &constraintcontactpoint_Layout::EventButtonCloseConfigPopupClick   );

	// Helper Button
	mButtonHoverHelp.Caption = "?";
	mButtonHoverHelp.Enabled = false;
	mButtonHoverHelp.PropertyList.Find( "Hint" )->SetString( "Hover options for extra info" );

	// Title Label
	mPopupTitleLabel.Caption = "Tool Preferences";
	mPopupTitleLabel.Style = kFBTextStyleBold;

	// New Markers Label
	mPopupMarkersLabel.Caption = "New markers:";

	// Checkbox for Auto-key
	mButtonAutoKeyCheck.Caption	= "Auto Key";
	mButtonAutoKeyCheck.Style = kFBCheckbox;
	mButtonAutoKeyCheck.State = mConstraint->GetAutoKey() ? 1 : 0;
	mButtonAutoKeyCheck.PropertyList.Find( "Hint" )->SetString( "Add full-weight key on new node and\nzero the weight of the last marker" );
	mButtonAutoKeyCheck.OnClick.Add( this, (FBCallback)&constraintcontactpoint_Layout::EventButtonAutoKeyCheckClick );

	// Checkbox for Auto-Select (UI)
	mButtonAutoSelectCheck.Caption	= "Start Selected";
	mButtonAutoSelectCheck.Style = kFBCheckbox;
	mButtonAutoSelectCheck.State = mConstraint->GetAutoSelect() ? 1 : 0;
	mButtonAutoSelectCheck.PropertyList.Find( "Hint" )->SetString( "New markers appear on the list already\nselected and ic focus" );
	mButtonAutoSelectCheck.OnClick.Add( this, (FBCallback)&constraintcontactpoint_Layout::EventButtonAutoSelectCheckClick );

	// Interface Label
	mPopupSelectLabel.Caption = "Interface:";

	// Checkbox for Auto-Select (Scene)
	mButtonSceneSelectCheck.Caption	= "Auto Select";
	mButtonSceneSelectCheck.Style = kFBCheckbox;
	mButtonSceneSelectCheck.State = mConstraint->GetSceneSelect() ? 1 : 0;
	mButtonSceneSelectCheck.PropertyList.Find( "Hint" )->SetString( "Clicking a list item selects it in the scene" );
	mButtonSceneSelectCheck.OnClick.Add( this, (FBCallback)&constraintcontactpoint_Layout::EventButtonSceneSelectCheckClick );

	// Checkbox for Persistent Selection
	mButtonPersistentSelectCheck.Caption	= "Keep Selected";
	mButtonPersistentSelectCheck.Style = kFBCheckbox;
	mButtonPersistentSelectCheck.State = mConstraint->GetPersistentSelect() ? 1 : 0;
	mButtonPersistentSelectCheck.PropertyList.Find( "Hint" )->SetString( "Keep selected marker in focus even\nwhen constraint is not in view" );
	mButtonPersistentSelectCheck.OnClick.Add( this, (FBCallback)&constraintcontactpoint_Layout::EventButtonPersistentSelectCheckClick );
	
	
	// Radio
	mPopupDisplayLabel.Caption = "Markers Display:";

	mButtonRadioDispAll.Caption = "All";
	mButtonRadioDispAll.Style =	kFBRadioButton;
	mButtonRadioDispAll.State =	0;
	mButtonRadioDispAll.PropertyList.Find( "Hint" )->SetString( "Every created marker is visible in viewport" );
	mButtonRadioDispAll.OnClick.Add( this, (FBCallback) &constraintcontactpoint_Layout::EventButtonRadioDispAllClick );
	
	mButtonRadioDispSel.Caption = "Selected";
	mButtonRadioDispSel.Style =	kFBRadioButton;
	mButtonRadioDispSel.State =	1;
	mButtonRadioDispSel.PropertyList.Find( "Hint" )->SetString( "Only selected list item is visible in viewport" );
	mButtonRadioDispSel.OnClick.Add( this, (FBCallback) &constraintcontactpoint_Layout::EventButtonRadioDispSelClick );

	mButtonRadioDispNil.Caption = "None";
	mButtonRadioDispNil.Style =	kFBRadioButton;
	mButtonRadioDispNil.State =	0;
	mButtonRadioDispNil.PropertyList.Find( "Hint" )->SetString( "Do not display any marker" );
	mButtonRadioDispNil.OnClick.Add( this, (FBCallback) &constraintcontactpoint_Layout::EventButtonRadioDispNilClick );

	//Set ui display marker type
	SetMarkerDisplayType( mConstraint->GetDisplayMarkerType() );

	// radio for find key type
	mPopupKeyframeLabel.Caption = "Next/Prev Keyframe:";

	mButtonRadioFindAll.Caption = "All";
	mButtonRadioFindAll.Style =	kFBRadioButton;
	mButtonRadioFindAll.State =	1;
	mButtonRadioFindAll.PropertyList.Find( "Hint" )->SetString( "Search for any nearest weight keyframe" );
	mButtonRadioFindAll.OnClick.Add( this, (FBCallback) &constraintcontactpoint_Layout::EventButtonRadioFindAllClick );
	
	mButtonRadioFindNon.Caption = "Non-zero";
	mButtonRadioFindNon.Style =	kFBRadioButton;
	mButtonRadioFindNon.State =	0;
	mButtonRadioFindNon.PropertyList.Find( "Hint" )->SetString( "Ignore zero-weight keyframes when searching" );
	mButtonRadioFindNon.OnClick.Add( this, (FBCallback) &constraintcontactpoint_Layout::EventButtonRadioFindNonClick );

	// set it
	SetFindKeyframeType( mConstraint->GetFindAllKeyframes() );

	// reset
	mPopupResetLabel.Caption = "Reset?";
	
	mButtonReset.Caption = "Reset Tool";
	mButtonReset.PropertyList.Find( "Hint" )->SetString( "Delete auxiliary rig and markers" );
	mButtonReset.OnClick.Add( this, (FBCallback) &constraintcontactpoint_Layout::EventButtonResetClick );


	UIConfigurePopupAbout();
}



/************************************************
 *	Configure & add callbacks to the UI Elements (for Popup).
 ************************************************/
void constraintcontactpoint_Layout::UIConfigurePopupAbout()
{
	mPopupAbout.Caption = "About";

	mButtonCloseAbout.Caption = "X";
	mButtonCloseAbout.PropertyList.Find( "Hint" )->SetString( "Close window" );
	mButtonCloseAbout.OnClick.Add	( this, (FBCallback) &constraintcontactpoint_Layout::EventButtonCloseAboutPopupClick   );

	mPopupInfo1Label.Caption = "Contact Point Editing Tool\nVersion 1.03 (Beta)";

    mPopupInfo2Label.Caption = "Instructions:";
    mPopupInfo2Label.Style = kFBTextStyleItalic;

	mPopupInfo3Label.Caption =	"Drag and drop the root of bone\n"
								"hierarchy you wish to control.\n"
								"After set-up, create contact markers\n"
								"by dragging correspondent body\n"
								"parts (bones) to the list.\n"
								"Plot to root when satisfied.\n"
								"Demonstration video:";

    mPopupInfoLinkText.Text = VIMEO_DEMO_LINK;
    mPopupInfoLinkText.OnChange.Add( this, (FBCallback) &constraintcontactpoint_Layout::EventChangeAboutLink ); //maintain link as is
    
    mPopupInfo4Label.Caption = "Author:";
    mPopupInfo4Label.Style = kFBTextStyleItalic;
 
	mPopupInfo5Label.Caption = "C++ Plugin written by\nScatena - Apr 2017\ninfo@scatena.tv";
}


/************************************************
 *	Reset the UI from the constraint values.
 ************************************************/
void constraintcontactpoint_Layout::UIReset()
{
	/* debug 
	FBMessageBox("Tool test", "ui reset call", "OK", NULL, NULL, 1 );
	/**/

	// check for setup or not
	locIsSetup = mConstraint->GetSetUp();

	// start disabled
	// ui setup button - disabled until set
	UISetupEnable( false );
	// disable root list after setuo
	UIRootDisable ( locIsSetup );
	// root name input list - only populate it if is empty
	if ( mListRoot.Items.GetCount() == 0 )
	{
		if ( mConstraint->GetRootString().GetLen() == 0 )
			mListRoot.Items.Add( "---Insert Root Bone---",	NULL );
		else
		{
			mListRoot.Items.Add( mConstraint->GetRootString(),	NULL );
			// if there is model and has not been setup, enable setup button
			if ( !locIsSetup )
				UISetupEnable( true );
		}
	}
	// just enable model list in case there is model configured
	UIListEnable( locIsSetup );

	// marker property + del button - start disabled
	SetupMarkerProperty( -1 );
	if ( locIsSetup )
	{
		// get added list nodes
		int lCount = mConstraint->GetNodeCount();
		
		// reset model list
		mListModel.Items.Clear();
		// if there are no nodes, add instructions only
		if( lCount == 0 )
			mListModel.Items.Add( "--- Add Pivots ---" );
		// else, procced
		else
		{
			// iterate over list items
			for( int i=0; i<lCount; i++ )
			{
				// add item to list - no longer using extra kReference
				mListModel.Items.Add( mConstraint->GetNodeName( i ) );
			}

			// se estiver em permanente
			if ( mButtonPersistentSelectCheck.State == 1 )
			{
				mSelectedNodeIndex = mConstraint->GetListSelIndex();

				// if there is selected (permanent) item
				if ( mSelectedNodeIndex >= 0 )
				{
					// re-select item list
					mListModel.Selected( mSelectedNodeIndex, true );

					// marker property setup - using broad function - already has SetupMarkerProperty in it
					SetSelectedMarker( mSelectedNodeIndex );

					// markers display - if display selected
					if ( mButtonRadioDispSel.State == 1 )
						mConstraint->ShowMarker( mSelectedNodeIndex );
				}
			}
		}
	}
}

void constraintcontactpoint_Layout::UISetupEnable( bool pEnable )
{
	// only enable button when a valid node is set
	mButtonSetup.Enabled       = pEnable;
}

void constraintcontactpoint_Layout::UIRootDisable( bool pDisable )
{
	// do not enable root list if it is already configured
	mListRoot.Enabled		= !pDisable;
}

void constraintcontactpoint_Layout::UIListEnable( bool pIsSetup )
{
	// just enable model list in case there is model configured
	mListModel.Enabled				= pIsSetup;
	mButtonPrevNode.Enabled			= pIsSetup;
	mButtonFindNode.Enabled			= pIsSetup;
	mButtonNextNode.Enabled			= pIsSetup;
	// not here
	// mButtonDelNode.Enabled			= pIsSetup;
	//mMarkerProperty.Enabled			= pIsSetup;
}




/************************************************
 *	Root List callback.
 ************************************************/

void constraintcontactpoint_Layout::EventRootListDragAndDrop( HISender pSender, HKEvent pEvent )
{

	FBEventDragAndDrop lDragAndDrop( pEvent );
    
	switch( lDragAndDrop.State )
    {
        case kFBDragAndDropDrag:
        {
            lDragAndDrop.Accept();
        }
        break;
        case kFBDragAndDropDrop:
        {
			FBComponent* lComponent = lDragAndDrop.Get(0);
			if( lComponent )
			{
				// clear first
				mListRoot.Items.Clear();
				// if is skeleton
				if( lComponent->Is( FBModelSkeleton::TypeInfo ) )
				{
					//add item
					mListRoot.Items.Add( lComponent->LongName, 0 );
					UISetupEnable( true );
					//check to see if is root bone - warn if not
					if ( !IsRootBone( (FBModel*)lComponent ) )
						FBMessageBox("Warning", "Bone is not the root.\nThis might cause issues.", "OK", NULL, NULL, 1 );
					// send the node
					mConstraint->SetRootNode( (FBModel*)lComponent );
				}
				// if not, add instructions
				else
					mListRoot.Items.Add( "---Must be Skeleton Node---",	NULL );
			}
        }
        break;
    }
}

void constraintcontactpoint_Layout::EventRootListDeselect( HISender pSender, HKEvent pEvent )
{
	//always keep list de-selected (UX purposes only)
	mListRoot.Selected(0, false);
}


void constraintcontactpoint_Layout::EventModelListDragAndDrop( HISender pSender, HKEvent pEvent )
{
	FBEventDragAndDrop lDragAndDrop( pEvent );
    
	switch( lDragAndDrop.State )
    {
        case kFBDragAndDropDrag:
        {
            lDragAndDrop.Accept();
        }
        break;
        case kFBDragAndDropDrop:
		{
			int lAddedNodes = 0;
			FBComponent* lComponent = NULL;
			int lCount = lDragAndDrop.GetCount();
			int lFirstIndex = 0;
			// if is auto key and more and one is selected
			if ( mButtonAutoKeyCheck.State && lCount > 1 )
			{
				// get user option
				int lUserOption = FBMessageBox("Warning", "Multiple items selected while autokey is on.\nThis can cause unexpected behaviour.", "Proceed", "Disable A-Key", "Use last selec.");
				

				// disable autokey
				if ( lUserOption == 2 )
				{
					mConstraint->SetAutoKey( false );
					mButtonAutoKeyCheck.State = 0;
				}
				// get last selected
				else if ( lUserOption == 3 )
				{
					lFirstIndex = lCount-1;
				}
			}
			// iterate over dragged items
			for(int i = lFirstIndex;i<lCount;i++)
			{
				lComponent = lDragAndDrop.Get(i);
				// try to add component - via constraint
				lAddedNodes += mConstraint->AddComponentNode( lComponent );
			}
			// Update if needed
			if ( lAddedNodes > 0 )
				UIReset();
		}
        break;
    }
}

void constraintcontactpoint_Layout::EventModelListClick( HISender pSender, HKEvent pEvent )
{
	// se a lista só tem instrução - mantém a instrução deselecionada
	if ( mConstraint->GetNodeCount() == 0 )
		mListModel.Selected(0, false);
	// se houver nodes
	else
	{
		// reseta
		SetSelectedMarker(-1);
		// if display selected - hide all
		if ( mButtonRadioDispSel.State == 1 )
		{
			mConstraint->SetMarkersVisibility( false );
		}
		
		// pega-se o índice selecionado
		int selIndex = mListModel.ItemIndex;
		// no primeiro clique, registra qual foi o selecionado - se ele já era selecionado antes, deseleciona
		if ( selIndex != mSelectedNodeIndex )
		{
			SetSelectedMarker(selIndex);
			// marca como selecionado e registra que o foi
			mListModel.Selected(selIndex, true);
			mSelectedNodeIndex = selIndex;
			// control display - selected
			if ( mButtonRadioDispSel.State == 1 )
				mConstraint->ShowMarker(selIndex );
		}
		else
		{
			// marca como deselecionado e tira o registro de seleção
			mListModel.Selected(selIndex, false);
			mSelectedNodeIndex = -1;
			// control display
		}
		// if persistent
		if ( mButtonPersistentSelectCheck.State == 1 )
		{
			mConstraint->SetListSelIndex( mSelectedNodeIndex );
		}
	}
}

void constraintcontactpoint_Layout::SetSelectedMarker( int pSelIndex )
{
	if ( pSelIndex < 0 )
	{
		// deselect mode
		// defocus all
		mConstraint->DefocusAll();
		// se estiver setado pra lista selecionar o marker
		if ( mButtonSceneSelectCheck.State == 1 )
			mConstraint->DeselectAll();
	}
	else
	{
		//modo de seleção
		// seta o foco
		mConstraint->FocusMarker( pSelIndex );
		// se estiver setado pra lista selecionar o marker
		if ( mButtonSceneSelectCheck.State == 1 )
			mConstraint->SelectMarker( pSelIndex ); // more elegant
	}
	// marker property setup
	SetupMarkerProperty( pSelIndex );

}

/************************************************
 *	Button click callback.
 ************************************************/
void constraintcontactpoint_Layout::EventButtonSetupClick( HISender pSender, HKEvent pEvent )
{
	// Setup auxiliary rig

	// via Constraint function
	if ( !mConstraint->SetUpAuxRig() )
		FBMessageBox("Error", "Something went bad. Node could not be set up.", "OK", NULL, NULL, 1 );

	// do not call any layout function ( like UIReset() ), or else program will crash
	// UI will be properly refreshed when copied hierarchy is brought in via merge
	
}

// extras

// list callbakcs








void constraintcontactpoint_Layout::EventButtonAutoKeyCheckClick ( HISender pSender, HKEvent pEvent )
{
	// active reproduces the state of button
	//mConstraint->Active = ( mButtonAutoKeyCheck.State == 1) ;
	//make internal activation
	//mConstraint->SetActivateCnt( mButtonAutoKeyCheck.State );
	mConstraint->SetAutoKey( mButtonAutoKeyCheck.State==1 );
	
}

void constraintcontactpoint_Layout::EventButtonAutoSelectCheckClick ( HISender pSender, HKEvent pEvent )
{
	mConstraint->SetAutoSelect( mButtonAutoSelectCheck.State==1 );
}


void constraintcontactpoint_Layout::EventButtonSceneSelectCheckClick ( HISender pSender, HKEvent pEvent )
{
	mConstraint->SetSceneSelect( mButtonSceneSelectCheck.State==1 );
}

void constraintcontactpoint_Layout::EventButtonPersistentSelectCheckClick ( HISender pSender, HKEvent pEvent )
{
	mConstraint->SetPersistentSelect( mButtonPersistentSelectCheck.State==1 );
	// se for setado pra sim, guardar qual a seleção
	if ( mButtonPersistentSelectCheck.State==1 )
		mConstraint->SetListSelIndex( mSelectedNodeIndex );
}

//radio
void constraintcontactpoint_Layout::EventButtonRadioDispAllClick ( HISender pSender, HKEvent pEvent )
{
	SetMarkerDisplayType( DISPLAYALL );
	mConstraint->SetDisplayMarkerType( DISPLAYALL );
}

void constraintcontactpoint_Layout::EventButtonRadioDispSelClick ( HISender pSender, HKEvent pEvent )
{
	SetMarkerDisplayType( DISPLAYSEL );
	mConstraint->SetDisplayMarkerType( DISPLAYSEL );
}

void constraintcontactpoint_Layout::EventButtonRadioDispNilClick ( HISender pSender, HKEvent pEvent )
{
	SetMarkerDisplayType( DISPLAYNIL );
	mConstraint->SetDisplayMarkerType( DISPLAYNIL );
}


// radio keyframe
void constraintcontactpoint_Layout::EventButtonRadioFindAllClick ( HISender pSender, HKEvent pEvent )
{
	SetFindKeyframeType( true );
	mConstraint->SetFindAllKeyframes( true );
}

void constraintcontactpoint_Layout::EventButtonRadioFindNonClick ( HISender pSender, HKEvent pEvent )
{
	SetFindKeyframeType( false );
	mConstraint->SetFindAllKeyframes( false );
}

void constraintcontactpoint_Layout::SetFindKeyframeType( bool pBool )
{
	mButtonRadioFindAll.State = pBool ? 1 : 0;
	mButtonRadioFindNon.State = pBool ? 0 : 1;
}



void constraintcontactpoint_Layout::EventButtonDelNodeClick ( HISender pSender, HKEvent pEvent )
{
	if ( FBMessageBox("Warning", "Are you sure?\nThis is not undoable.", "OK", "Cancel", NULL, 1 ) == 1 )
	{
		// deseleciona
		mListModel.Selected( -1, true);
		//ui
		SetupMarkerProperty( -1 );
	
		// remove do constraint
		mConstraint->DeleteNodeAt( mSelectedNodeIndex );


		UIReset();
	}
}


// next prev
void constraintcontactpoint_Layout::EventButtonPrevNodeClick ( HISender pSender, HKEvent pEvent )
{
	UISelectMarker( mConstraint->FindPrevKey() );
}


void constraintcontactpoint_Layout::EventButtonNextNodeClick ( HISender pSender, HKEvent pEvent )
{
	UISelectMarker( mConstraint->FindNextKey() );
}

void constraintcontactpoint_Layout::EventButtonFindNodeClick ( HISender pSender, HKEvent pEvent )
{
	UISelectMarker( mConstraint->FindWeight() );
}

void constraintcontactpoint_Layout::UISelectMarker( int pIndex )
{
	// if valid selection
	if ( pIndex >= 0 )
	{
		// select in list and register
		mListModel.Selected(pIndex, true);
		mSelectedNodeIndex = pIndex;
		// focus on it
		mConstraint->FocusMarker( pIndex );
		// set property
		SetupMarkerProperty( pIndex );
		// if scene select is active, select it in scene
		if ( mButtonSceneSelectCheck.State == 1 )
			mConstraint->SelectMarker( pIndex );
		// if persistent selection is active, send index value
		if ( mButtonPersistentSelectCheck.State == 1 )
		{
			mConstraint->SetListSelIndex( pIndex );
		}
	}
}

/************************************************
 *	Button config popup callback.
 ************************************************/
void constraintcontactpoint_Layout::EventButtonConfigPopupClick( HISender pSender, HKEvent pEvent )
{
	mPopupConfig.Modal = true;
	mPopupConfig.Show();
}
/************************************************
 *	Close config popup button callback.
 ************************************************/
void constraintcontactpoint_Layout::EventButtonCloseConfigPopupClick( HISender pSender, HKEvent pEvent )
{
	mPopupConfig.Close();
}

/************************************************
 *	Button about popup callback.
 ************************************************/
void constraintcontactpoint_Layout::EventButtonAboutPopupClick( HISender pSender, HKEvent pEvent )
{
	mPopupAbout.Modal = true;
	mPopupAbout.Show();
}
/************************************************
 *	Close about popup button callback.
 ************************************************/
void constraintcontactpoint_Layout::EventButtonCloseAboutPopupClick( HISender pSender, HKEvent pEvent )
{
	mPopupAbout.Close();
}
/************************************************
 *	Write back video link no matter what the user input is
 ************************************************/
void constraintcontactpoint_Layout::EventChangeAboutLink( HISender pSender, HKEvent pEvent )
{
	mPopupInfoLinkText.Text = VIMEO_DEMO_LINK;
}


void constraintcontactpoint_Layout::EventButtonResetClick( HISender pSender, HKEvent pEvent )
{
	// reset constraint
	// confirm dialog
	if ( FBMessageBox("Warning", "Are you sure?\nThis is not undoable.", "OK", "Cancel", NULL, 1 ) == 1 )
	{
		// clear UI focus and selection before deleting elements
			
		// select none for model list
		mListModel.Selected( -1, true);
		// set property null
		SetupMarkerProperty( -1 );
		// clear root list
		mListRoot.Items.Clear();
		// clear nodes list
		mListModel.Items.Clear();
	
		//mConstraint->ResetUI();
		mConstraint->ResetTool();

		// update UI
		UIReset();
	}
	
}



//ui



//move to cosntraint
// helpers
// Helpers



bool constraintcontactpoint_Layout::IsRootBone( FBModel* pModel )
{
	// temp? 
	// if there is no model, terminate
	if ( pModel == NULL )
		return false;

	// if parent exists...
	if ( pModel->Parent )
	{
		// ... and is skeleton, the model can't be root
		if ( pModel->Parent->Is( FBModelSkeleton::TypeInfo ) )
			return false;
	}
	return true;
		
}





void constraintcontactpoint_Layout::SetupMarkerProperty( int pIndex )
{
	if ( pIndex >= 0 )
	{
		// enable property for current marker
		FBProperty* lProp = mConstraint->GetMarkerAt(pIndex)->PropertyList.Find("Weight");
		mMarkerProperty.Property = lProp;
		mMarkerProperty.Enabled	= true;
		// activate delete button
		mButtonDelNode.Enabled = true;

	}
	else
	{
		//disable
		//dirty workaround - using base layer weight as property just for display
		FBProperty* lProp = FBSystem().CurrentTake->GetLayer(0)->PropertyList.Find("Weight");
		mMarkerProperty.Property = lProp;
		mMarkerProperty.Enabled = false;
		//deactivate delete button
		mButtonDelNode.Enabled = false;
		
	}
}


/************************************************
 *	Set UI selection type.
 ************************************************/
void constraintcontactpoint_Layout::SetMarkerDisplayType( int pType )
{
	bool	lDisplayAll	= false;
	bool	lDisplaySel	= false;
	bool	lDisplayNil	= false;

	switch( pType )
	{
		case DISPLAYALL:
			lDisplayAll	= true;
		break;
		case DISPLAYSEL:
			lDisplaySel	= true;
		break;
		case DISPLAYNIL:
			lDisplayNil	= true;
		break;
	}

	mButtonRadioDispAll.State = lDisplayAll ? 1 : 0;

	mButtonRadioDispSel.State = lDisplaySel ? 1 : 0;

	mButtonRadioDispNil.State = lDisplayNil ? 1 : 0;

	// refresh visibility
	// iterate through all
	mConstraint->SetMarkersVisibility( lDisplayAll );
	if ( lDisplaySel )
	{
		// show only selected - if any
		if ( mSelectedNodeIndex >= 0 )
			mConstraint->ShowMarker( mSelectedNodeIndex );
	}
}

/* -- Development helper function
void constraintcontactpoint_Layout::DebugIntMessage( int pNumber )
{
	char name[50];
	sprintf(name,"Number is: %i", pNumber);
	FBMessageBox("Message", name, "OK", NULL, NULL, 1 );
}
*/
