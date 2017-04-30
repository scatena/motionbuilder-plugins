#ifndef __constraintcontactpoint_LAYOUT_H__
#define __constraintcontactpoint_LAYOUT_H__
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

/**	\file	constraintcontactpoint_layout.h
*	Declaration of a layout class for a simple constraint.
*	Declaration of the FBSimpleConstraintLayout class.
*/

//--- SDK include
#include <fbsdk/fbsdk.h>

//--- Class declaration
#include "constraintcontactpoint_constraint.h"

//--- Link for Demonstration
#define VIMEO_DEMO_LINK		"https://vimeo.com/213600960"

//! Simple constraint layout.
class constraintcontactpoint_Layout : public FBConstraintLayout
{
	//--- constraint layout declaration.
	FBConstraintLayoutDeclare( constraintcontactpoint_Layout, FBConstraintLayout );

public:
	virtual bool FBCreate();
	virtual void FBDestroy();

private:
	// UI Management
	void	UICreate	();
	void	UICreatePopupConfig();
	void	UICreatePopupAbout();
	void	UIConfigure	();
	void	UIConfigurePopupConfig();
	void	UIConfigurePopupAbout();
	void	UIReset		();

	// UI Callbacks
	// Root list
	void	EventRootListDragAndDrop	( HISender pSender, HKEvent pEvent );
	void	EventRootListDeselect	( HISender pSender, HKEvent pEvent );
	
	// Setup Button
	void	EventButtonSetupClick( HISender pSender, HKEvent pEvent );

	// Reference models list
	void	EventModelListDragAndDrop( HISender pSender, HKEvent pEvent );
	void	EventModelListClick( HISender pSender, HKEvent pEvent );

	// Navigation & Delete Buttons
	void	EventButtonPrevNodeClick( HISender pSender, HKEvent pEvent );
	void	EventButtonNextNodeClick( HISender pSender, HKEvent pEvent );
	void	EventButtonFindNodeClick( HISender pSender, HKEvent pEvent );
	void	EventButtonDelNodeClick( HISender pSender, HKEvent pEvent );

	// 'Config' Popup
	void	EventButtonConfigPopupClick		( HISender pSender, HKEvent pEvent );
	void	EventButtonCloseConfigPopupClick	( HISender pSender, HKEvent pEvent );

	void	EventButtonAutoKeyCheckClick( HISender pSender, HKEvent pEvent );
	void	EventButtonAutoSelectCheckClick( HISender pSender, HKEvent pEvent );
	void	EventButtonSceneSelectCheckClick( HISender pSender, HKEvent pEvent );
	void	EventButtonPersistentSelectCheckClick( HISender pSender, HKEvent pEvent );

	void	EventButtonRadioDispAllClick( HISender pSender, HKEvent pEvent );
	void	EventButtonRadioDispSelClick( HISender pSender, HKEvent pEvent );
	void	EventButtonRadioDispNilClick( HISender pSender, HKEvent pEvent );

	void	EventButtonRadioFindAllClick( HISender pSender, HKEvent pEvent );
	void	EventButtonRadioFindNonClick( HISender pSender, HKEvent pEvent );
	
	void	EventButtonResetClick	( HISender pSender, HKEvent pEvent );

	// 'About' popup
	void	EventButtonAboutPopupClick		( HISender pSender, HKEvent pEvent );
	void	EventButtonCloseAboutPopupClick	( HISender pSender, HKEvent pEvent );
	
	void	EventChangeAboutLink	( HISender pSender, HKEvent pEvent );

	// Handle
	constraintcontactpoint*	mConstraint;

	// Main UI Elements
	FBLabel			mTitleLabel;
	
	FBList			mListRoot;

	FBButton		mButtonAbout;
	FBButton		mButtonConfig;
	FBButton		mButtonSetup;

	FBList			mListModel;

	FBButton		mButtonPrevNode;
    FBButton		mButtonFindNode;
	FBButton		mButtonNextNode;

	FBEditProperty	mMarkerProperty;

	FBButton		mButtonDelNode;
	
	// 'Config' Popup Elements
	FBPopup			mPopupConfig;

	FBButton		mButtonCloseConfig;
	FBButton		mButtonHoverHelp;
	FBLabel			mPopupTitleLabel;

	FBLabel			mPopupMarkersLabel;
	FBButton		mButtonAutoKeyCheck;
	FBButton		mButtonAutoSelectCheck;

	FBLabel			mPopupSelectLabel;
	FBButton		mButtonSceneSelectCheck;
	FBButton		mButtonPersistentSelectCheck;

	FBLabel			mPopupDisplayLabel;
	FBButton		mButtonRadioDispAll;
	FBButton		mButtonRadioDispSel;
	FBButton		mButtonRadioDispNil;

	FBLabel			mPopupKeyframeLabel;
	FBButton		mButtonRadioFindAll;
	FBButton		mButtonRadioFindNon;

	FBLabel			mPopupResetLabel;
	FBButton		mButtonReset;

	// 'Info' Popup Elements
	FBPopup			mPopupAbout;

	FBButton		mButtonCloseAbout;
	FBLabel			mPopupInfo1Label;
	FBLabel			mPopupInfo2Label;
	FBLabel			mPopupInfo3Label;
	FBEdit			mPopupInfoLinkText;
	FBLabel			mPopupInfo4Label;
	FBLabel			mPopupInfo5Label;
	
	// UI Management Auxiliary Objects
	int			mSelectedNodeIndex;
	bool		locIsSetup;

	// Helper functions for UI Management
	void		UISetupEnable( bool pEnable );
	void		UIRootDisable( bool pEnable );
	void		UIListEnable( bool pEnable );
		
	void		SetupMarkerProperty( int pIndex );

	void		SetFindKeyframeType( bool pBool );

	void		SetMarkerDisplayType( int pType );	
	
	void		UISelectMarker( int pIndex );
	
	// Helper Functions - Specifics
	bool		IsRootBone( FBModel* pModel );

	void		SetSelectedMarker( int pSelIndex );

	/* -- Development helper function
	void	DebugIntMessage( int pNumber );
	*/

};

#endif /* __constraintcontactpoint_LAYOUT_H__ */
