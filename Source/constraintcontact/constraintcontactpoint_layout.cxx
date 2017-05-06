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
	
	// Radio buttons for Marker Display
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

	// Set UI from constraint data
	SetMarkerDisplayType( mConstraint->GetDisplayMarkerType() );

	// Radio Buttons for Navigation Options (Find Keyframe)
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

	// Set UI from constraint data
	SetFindKeyframeType( mConstraint->GetFindAllKeyframes() );

	// 'Reset Tool' Button
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
	// Popup Title
	mPopupAbout.Caption = "About";

	// 'Close' Button
	mButtonCloseAbout.Caption = "X";
	mButtonCloseAbout.PropertyList.Find( "Hint" )->SetString( "Close window" );
	mButtonCloseAbout.OnClick.Add	( this, (FBCallback) &constraintcontactpoint_Layout::EventButtonCloseAboutPopupClick   );

	// Title Label
	mPopupInfo1Label.Caption = "Contact Point Editing Tool\nVersion 1.03 (Beta)";

	// Instructions Labels
    mPopupInfo2Label.Caption = "Instructions:";
    mPopupInfo2Label.Style = kFBTextStyleItalic;

	mPopupInfo3Label.Caption =	"Drag and drop the root of bone\n"
								"hierarchy you wish to control.\n"
								"After set-up, create contact markers\n"
								"by dragging correspondent body\n"
								"parts (bones) to the list.\n"
								"Plot to root when satisfied.\n"
								"Demonstration video:";

	// Link text for video demonstration
    mPopupInfoLinkText.Text = VIMEO_DEMO_LINK;
    mPopupInfoLinkText.OnChange.Add( this, (FBCallback) &constraintcontactpoint_Layout::EventChangeAboutLink ); //maintain link as is
    
	// Information Labels
    mPopupInfo4Label.Caption = "Author:";
    mPopupInfo4Label.Style = kFBTextStyleItalic;
 
	mPopupInfo5Label.Caption = "C++ Plugin written by\nScatena - Apr 2017\ninfo@scatena.tv";
}


/************************************************
 *	Reset the UI calling for constraint data
 ************************************************/
void constraintcontactpoint_Layout::UIReset()
{
	// Get set-up status from constraint
	locIsSetup = mConstraint->GetSetUp();

	// Set UI to begin disabled until tool is set
	UISetupEnable( false );
	// Disable root list if has been already set
	UIRootDisable ( locIsSetup );
	// Populate root input list - only if it is empty
	if ( mListRoot.Items.GetCount() == 0 )
	{
		// If there is no root model set, add instructions
		if ( mConstraint->GetRootString().GetLen() == 0 )
			mListRoot.Items.Add( "---Insert Root Bone---",	NULL );
		// Else add the models name
		else
		{
			mListRoot.Items.Add( mConstraint->GetRootString(),	NULL );
			// In case it has not been setup, enable Setup button
			if ( !locIsSetup )
				UISetupEnable( true );
		}
	}
	// Enable model list in case tool is set up
	UIListEnable( locIsSetup );

	// Marker-related UI (Navigation tools, property view and Delete button) will first be disabled...
	SetupMarkerProperty( -1 );
	// ...but in case it has been set up:
	if ( locIsSetup )
	{
		// Get added nodes count
		int lCount = mConstraint->GetNodeCount();
		
		// Reset the nodes list (UI)
		mListModel.Items.Clear();
		// If there are no nodes, add instructions
		if( lCount == 0 )
			mListModel.Items.Add( "--- Add Pivots ---" );
		// Else...
		else
		{
			// ...iterate over list items...
			for( int i=0; i<lCount; i++ )
			{
				// ...and add it list.
				mListModel.Items.Add( mConstraint->GetNodeName( i ) );
			}

			// Status check - If set to 'Persistent selection'
			if ( mButtonPersistentSelectCheck.State == 1 )
			{
				// Get which item is selected from tool...
				mSelectedNodeIndex = mConstraint->GetListSelIndex();

				// ...and if there is any...
				if ( mSelectedNodeIndex >= 0 )
				{
					// ...re-select it on the list...
					mListModel.Selected( mSelectedNodeIndex, true );

					// ...and set its property to be viewed (using helper function that makes use of SetupMarkerProperty()
					SetSelectedMarker( mSelectedNodeIndex );

					// Status check - If set to 'Display Selected'...
					if ( mButtonRadioDispSel.State == 1 )
						// ...show it.
						mConstraint->ShowMarker( mSelectedNodeIndex );
				}
			}
		}
	}
}


/************************************************
 *	Root List Drag and Drop callback.
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
			// Grab dropped component
			FBComponent* lComponent = lDragAndDrop.Get(0);
			if( lComponent )
			{
				// Clear list
				mListRoot.Items.Clear();
				// If component is skeleton...
				if( lComponent->Is( FBModelSkeleton::TypeInfo ) )
				{
					// Add item to list
					mListRoot.Items.Add( lComponent->LongName, 0 );
					// Enable Set-up button;
					UISetupEnable( true );
					// Check if node is root bone - warn if not
					if ( !IsRootBone( (FBModel*)lComponent ) )
						FBMessageBox("Warning", "Bone is not the root.\nThis might cause issues.", "OK", NULL, NULL, 1 );
					// Send the node to constraint
					mConstraint->SetRootNode( (FBModel*)lComponent );
				}
				// IF component is not skeleton, add new instructions
				else
					mListRoot.Items.Add( "---Must be Skeleton Node---",	NULL );
			}
        }
        break;
    }
}


/************************************************
 *	Root List Click callback.
 ************************************************/
void constraintcontactpoint_Layout::EventRootListDeselect( HISender pSender, HKEvent pEvent )
{
	// Always keep list deselected (UX purposes only)
	mListRoot.Selected(0, false);
}


/************************************************
 *	Model List Drag and Drop callback.
 ************************************************/
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
			// Initialize a counter for nodes to be accepted
			int lAddedNodes = 0;
			FBComponent* lComponent = NULL;
			// Get how many components were dropped
			int lCount = lDragAndDrop.GetCount();
			int lFirstIndex = 0;
			// Status check - If set to 'Auto Key' and user tries to add multiple components...
			if ( mButtonAutoKeyCheck.State && lCount > 1 )
			{
				// Ask for user option
				switch( FBMessageBox("Warning", "Multiple items selected while autokey is on.\nThis can cause unexpected behaviour.", "Proceed", "Disable A-Key", "Use last selec.") )
				{
					// Case 'Disable Autokey'
					case 2:
					{
						mConstraint->SetAutoKey( false );
						mButtonAutoKeyCheck.State = 0;
					}
					break;
					// Case 'Last Selected'
					case 3:
					{
						lFirstIndex = lCount-1;
					}
					break;
				}
			}
			// Iterate over dragged items
			for( int i = lFirstIndex; i<lCount; i++ )
			{
				// Grab respective component
				lComponent = lDragAndDrop.Get(i);
				// Try to add component - via constraint
				lAddedNodes += mConstraint->AddComponentNode( lComponent );
			}
			// Update if needed (at least one node successfully added)
			if ( lAddedNodes > 0 )
				UIReset();
		}
        break;
    }
}


/************************************************
 *	Model List Click callback.
 ************************************************/
void constraintcontactpoint_Layout::EventModelListClick( HISender pSender, HKEvent pEvent )
{
	// If list has only instructions, keep deselect (UX purposes only)
	if ( mConstraint->GetNodeCount() == 0 )
		mListModel.Selected(0, false);
	// Otherwise, if has nodes
	else
	{
		// Reset UI List Selection
		SetSelectedMarker(-1);
		// Status check - If set to 'Display Selected', hide all
		if ( mButtonRadioDispSel.State == 1 )
		{
			mConstraint->SetMarkersVisibility( false );
		}
		
		// Grab which index was clicked
		int selIndex = mListModel.ItemIndex;
		// Compare clicked index with which index was thought to be selected (if any)
		// If indexes are different...
		if ( selIndex != mSelectedNodeIndex )
		{
			// Focus the marker and its properties
			SetSelectedMarker(selIndex);
			// Select the item and register its index
			mListModel.Selected(selIndex, true);
			mSelectedNodeIndex = selIndex;
			// Status check - If set to 'Display Selected', show clicked node
			if ( mButtonRadioDispSel.State == 1 )
				mConstraint->ShowMarker(selIndex );
		}
		// If indexes are the same...
		else
		{
			// Deselect and unregister its index
			mListModel.Selected(selIndex, false);
			mSelectedNodeIndex = -1;
		}
		// Status check - If set to 'Persistent Selection'
		if ( mButtonPersistentSelectCheck.State == 1 )
		{
			// Register in the constraint which index was now selected (-1 in the case of none)
			mConstraint->SetListSelIndex( mSelectedNodeIndex );
		}
	}
}


/************************************************
 *	Setup Button Click callback.
 ************************************************/
void constraintcontactpoint_Layout::EventButtonSetupClick( HISender pSender, HKEvent pEvent )
{
	// Setup auxiliary rig - via  Constraint function
	if ( !mConstraint->SetUpAuxRig() )
		FBMessageBox("Error", "Something went wrong. Node could not be set up.", "OK", NULL, NULL, 1 );

	// Warning: do not call any layout function ( like UIReset() ), or else program will crash.
	// UI will be properly refreshed when copied hierarchy is brought in via merge.
}


/************************************************
 *	Find Keyframe 'Previous' Button Click callback.
 ************************************************/
void constraintcontactpoint_Layout::EventButtonPrevNodeClick ( HISender pSender, HKEvent pEvent )
{
	UISelectMarker( mConstraint->FindPrevKey() );
}


/************************************************
 *	Find Keyframe 'Next' Button Click callback.
 ************************************************/
void constraintcontactpoint_Layout::EventButtonNextNodeClick ( HISender pSender, HKEvent pEvent )
{
	UISelectMarker( mConstraint->FindNextKey() );
}


/************************************************
 *	Find Keyframe 'Current' Button Click callback.
 ************************************************/
void constraintcontactpoint_Layout::EventButtonFindNodeClick ( HISender pSender, HKEvent pEvent )
{
	UISelectMarker( mConstraint->FindWeight() );
}


/************************************************
 *	Delete Node Button Click callback.
 ************************************************/
void constraintcontactpoint_Layout::EventButtonDelNodeClick ( HISender pSender, HKEvent pEvent )
{
	// Confirm with user
	if ( FBMessageBox("Warning", "Are you sure?\nThis is not undoable.", "OK", "Cancel", NULL, 1 ) == 1 )
	{
		// Deselect in list
		mListModel.Selected( -1, true);
		// Reset Property View
		SetupMarkerProperty( -1 );
		// Delete node from constraint
		mConstraint->DeleteNodeAt( mSelectedNodeIndex );

		// Update UI
		UIReset();
	}
}


/************************************************
 *	Open Popup 'Config' Button Click callback.
 ************************************************/
void constraintcontactpoint_Layout::EventButtonConfigPopupClick( HISender pSender, HKEvent pEvent )
{
	mPopupConfig.Modal = true;
	mPopupConfig.Show();
}


/************************************************
 *	Close Popup 'Config' Button Click callback.
 ************************************************/
void constraintcontactpoint_Layout::EventButtonCloseConfigPopupClick( HISender pSender, HKEvent pEvent )
{
	mPopupConfig.Close();
}


/************************************************
 *	Auto Key Check Button Click callback.
 ************************************************/
void constraintcontactpoint_Layout::EventButtonAutoKeyCheckClick ( HISender pSender, HKEvent pEvent )
{
	mConstraint->SetAutoKey( mButtonAutoKeyCheck.State==1 );
}


/************************************************
 *	Auto Select Check Button Click callback.
 ************************************************/
void constraintcontactpoint_Layout::EventButtonAutoSelectCheckClick ( HISender pSender, HKEvent pEvent )
{
	mConstraint->SetAutoSelect( mButtonAutoSelectCheck.State==1 );
}


/************************************************
 *	Scene Select Check Button Click callback.
 ************************************************/
void constraintcontactpoint_Layout::EventButtonSceneSelectCheckClick ( HISender pSender, HKEvent pEvent )
{
	mConstraint->SetSceneSelect( mButtonSceneSelectCheck.State==1 );
}


/************************************************
 *	Persistent Select Check Button Click callback.
 ************************************************/
void constraintcontactpoint_Layout::EventButtonPersistentSelectCheckClick ( HISender pSender, HKEvent pEvent )
{
	mConstraint->SetPersistentSelect( mButtonPersistentSelectCheck.State==1 );
	// If set to 'Persistent Select', register the selection in the constraint
	if ( mButtonPersistentSelectCheck.State==1 )
		mConstraint->SetListSelIndex( mSelectedNodeIndex );
}


/************************************************
 *	Display Marker 'All' Radio Button Click callback.
 ************************************************/
void constraintcontactpoint_Layout::EventButtonRadioDispAllClick ( HISender pSender, HKEvent pEvent )
{
	SetMarkerDisplayType( DISPLAYALL );
	mConstraint->SetDisplayMarkerType( DISPLAYALL );
}


/************************************************
 *	Display Marker 'Selected' Radio Button Click callback.
 ************************************************/
void constraintcontactpoint_Layout::EventButtonRadioDispSelClick ( HISender pSender, HKEvent pEvent )
{
	SetMarkerDisplayType( DISPLAYSEL );
	mConstraint->SetDisplayMarkerType( DISPLAYSEL );
}


/************************************************
 *	Display Marker 'None' Radio Button Click callback.
 ************************************************/
void constraintcontactpoint_Layout::EventButtonRadioDispNilClick ( HISender pSender, HKEvent pEvent )
{
	SetMarkerDisplayType( DISPLAYNIL );
	mConstraint->SetDisplayMarkerType( DISPLAYNIL );
}


/************************************************
 *	Find Keyframe 'All' Radio Button Click callback.
 ************************************************/
void constraintcontactpoint_Layout::EventButtonRadioFindAllClick ( HISender pSender, HKEvent pEvent )
{
	SetFindKeyframeType( true );
	mConstraint->SetFindAllKeyframes( true );
}


/************************************************
 *	Find Keyframe 'Non-Zero' Radio Button Click callback.
 ************************************************/
void constraintcontactpoint_Layout::EventButtonRadioFindNonClick ( HISender pSender, HKEvent pEvent )
{
	SetFindKeyframeType( false );
	mConstraint->SetFindAllKeyframes( false );
}


/************************************************
 *	Reset Tool Button Click callback.
 ************************************************/
void constraintcontactpoint_Layout::EventButtonResetClick( HISender pSender, HKEvent pEvent )
{
	// Confirm with user
	if ( FBMessageBox("Warning", "Are you sure?\nThis is not undoable.", "OK", "Cancel", NULL, 1 ) == 1 )
	{
		// Always clear UI focus and selection before deleting elements!
			
		// Set UI nodes list selection to none
		mListModel.Selected( -1, true);
		// Set property focus to none
		SetupMarkerProperty( -1 );
		// Clear root list
		mListRoot.Items.Clear();
		// Clear nodes list
		mListModel.Items.Clear();
		// Reset the constraint
		mConstraint->ResetTool();

		// Update UI
		UIReset();
	}
}


/************************************************
 *	Open Popup 'About' Button Click callback.
 ************************************************/
void constraintcontactpoint_Layout::EventButtonAboutPopupClick( HISender pSender, HKEvent pEvent )
{
	mPopupAbout.Modal = true;
	mPopupAbout.Show();
}


/************************************************
 *	Close Popup 'About' Button Click callback.
 ************************************************/
void constraintcontactpoint_Layout::EventButtonCloseAboutPopupClick( HISender pSender, HKEvent pEvent )
{
	mPopupAbout.Close();
}


/************************************************
 *	Demo Video Link On-Edit callback.
 ************************************************/
void constraintcontactpoint_Layout::EventChangeAboutLink( HISender pSender, HKEvent pEvent )
{
	// Write back video link no matter what the user has done with the text field
	mPopupInfoLinkText.Text = VIMEO_DEMO_LINK;
}


//   ########################
//   ########################
//		 Helper Functions
//   ########################
//   ########################


// Enable Setup Button
void constraintcontactpoint_Layout::UISetupEnable( bool pEnable )
{
	mButtonSetup.Enabled       = pEnable;
}


// Disable Root Input List
void constraintcontactpoint_Layout::UIRootDisable( bool pDisable )
{
	mListRoot.Enabled		= !pDisable;
}


// Enable Nodes Input List and Navigation Buttons
void constraintcontactpoint_Layout::UIListEnable( bool pIsSetup )
{
	mListModel.Enabled				= pIsSetup;
	mButtonPrevNode.Enabled			= pIsSetup;
	mButtonFindNode.Enabled			= pIsSetup;
	mButtonNextNode.Enabled			= pIsSetup;
}


// Set UI Property View
void constraintcontactpoint_Layout::SetupMarkerProperty( int pIndex )
{
	// If is a valid index
	if ( pIndex >= 0 )
	{
		// Enable property for current marker
		FBProperty* lProp = mConstraint->GetMarkerAt(pIndex)->PropertyList.Find("Weight");
		mMarkerProperty.Property = lProp;
		mMarkerProperty.Enabled	= true;
		// Activate Delete button
		mButtonDelNode.Enabled = true;
	}
	// Else, disable property view
	else
	{
		// Dirty workaroung -  display symbolic, locked property
		// Using base layer's weight as property just for display
		FBProperty* lProp = FBSystem().CurrentTake->GetLayer(0)->PropertyList.Find("Weight");
		mMarkerProperty.Property = lProp;
		mMarkerProperty.Enabled = false;
		// Deactivate Delete button
		mButtonDelNode.Enabled = false;
	}
}


// Set Navigation Serach Option - Helper for Exclusive Pick on Radio Button callbacks
void constraintcontactpoint_Layout::SetFindKeyframeType( bool pBool )
{
	mButtonRadioFindAll.State = pBool ? 1 : 0;
	mButtonRadioFindNon.State = pBool ? 0 : 1;
}


// Set Marker Display Option - Helper for Exclusive Pick on Radio Button callbacks
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

	// Refresh visibility for all markers
	mConstraint->SetMarkersVisibility( lDisplayAll );
	// If set to 'Display Selected"
	if ( lDisplaySel )
	{
		// Show selected - if any
		if ( mSelectedNodeIndex >= 0 )
			mConstraint->ShowMarker( mSelectedNodeIndex );
	}
}


// Select an marker in the interface (list and property display)
void constraintcontactpoint_Layout::UISelectMarker( int pIndex )
{
	// Check for valid index
	if ( pIndex >= 0 )
	{
		// Select list index and register the selection
		mListModel.Selected(pIndex, true);
		mSelectedNodeIndex = pIndex;
		// Focus the marker's property
		mConstraint->FocusMarker( pIndex );
		// Set Property View
		SetupMarkerProperty( pIndex );
		// Status check - If set to 'Scene Select', select it
		if ( mButtonSceneSelectCheck.State == 1 )
			mConstraint->SelectMarker( pIndex );
		// Status check - If set to 'Persistent selection', register index value
		if ( mButtonPersistentSelectCheck.State == 1 )
		{
			mConstraint->SetListSelIndex( pIndex );
		}
	}
}


// Check if model is root of skeleton hierarchy
bool constraintcontactpoint_Layout::IsRootBone( FBModel* pModel )
{
	// Safety check
	if ( pModel == NULL )
		return false;

	// If model has parent...
	if ( pModel->Parent )
	{
		// ...and it is a skeleton node, the model can't be the root
		if ( pModel->Parent->Is( FBModelSkeleton::TypeInfo ) )
			return false;
	}
	// Else, it probably is
	return true;
}


// Set Focus and Property view for chosen marker
void constraintcontactpoint_Layout::SetSelectedMarker( int pSelIndex )
{
	// Reset mode - null index
	if ( pSelIndex < 0 )
	{
		// Defocus all
		mConstraint->DefocusAll();
		// Status check - If set to 'Scene Select', deselect
		if ( mButtonSceneSelectCheck.State == 1 )
			mConstraint->DeselectAll();
	}
	// Selected marker mode
	else
	{
		// Focus its property 
		mConstraint->FocusMarker( pSelIndex );
		// Status check - If set to 'Scene Select', select it
		if ( mButtonSceneSelectCheck.State == 1 )
			mConstraint->SelectMarker( pSelIndex );
	}
	// Set Up Marker Property View
	SetupMarkerProperty( pSelIndex );
}


/* -- Development helper function
void constraintcontactpoint_Layout::DebugIntMessage( int pNumber )
{
	char name[50];
	sprintf(name,"Number is: %i", pNumber);
	FBMessageBox("Message", name, "OK", NULL, NULL, 1 );
}
*/
