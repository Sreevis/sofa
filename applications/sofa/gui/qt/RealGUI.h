/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 3      *
*                (c) 2006-2008 MGH, INRIA, USTL, UJF, CNRS                    *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU General Public License as published by the Free  *
* Software Foundation; either version 2 of the License, or (at your option)   *
* any later version.                                                          *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for    *
* more details.                                                               *
*                                                                             *
* You should have received a copy of the GNU General Public License along     *
* with this program; if not, write to the Free Software Foundation, Inc., 51  *
* Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.                   *
*******************************************************************************
*                            SOFA :: Applications                             *
*                                                                             *
* Authors: M. Adam, J. Allard, B. Andre, P-J. Bensoussan, S. Cotin, C. Duriez,*
* H. Delingette, F. Falipou, F. Faure, S. Fonteneau, L. Heigeas, C. Mendoza,  *
* M. Nesme, P. Neumann, J-P. de la Plata Alcade, F. Poyer and F. Roy          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef SOFA_GUI_VIEWER_REALGUI_H
#define SOFA_GUI_VIEWER_REALGUI_H

#ifdef SOFA_PML
#  include <sofa/filemanager/sofapml/PMLReader.h>
#  include <sofa/filemanager/sofapml/LMLReader.h>
#endif

#include <time.h>


#include <sofa/gui/SofaGUI.h>

#include "GUI.h"
#include <sofa/gui/qt/GraphListenerQListView.h>
#include <sofa/gui/qt/FileManagement.h>
#include <sofa/gui/qt/viewer/SofaViewer.h>
#include <sofa/gui/qt/AddObject.h>
#include <sofa/gui/qt/ModifyObject.h>
#include <sofa/simulation/tree/xml/XML.h>
#include <sofa/helper/system/SetDirectory.h>

#ifdef SOFA_QT4
#include <QApplication>
#include <QDesktopWidget>
#include <Q3ListViewItem>
#include <QStackedWidget>
#include <QSlider>
#include <QTimer>
#include <Q3TextDrag>
#include <Q3PopupMenu>
typedef Q3ListViewItem QListViewItem;
typedef QStackedWidget QWidgetStack;
typedef Q3PopupMenu QPopupMenu;
#else
typedef QTextDrag Q3TextDrag;
#include <qapplication.h>
#include <qdesktopwidget.h>
#include <qdragobject.h>
#include <qwidgetstack.h>
#include <qlistview.h>
#include <qslider.h>
#include <qpopupmenu.h>
#endif


namespace sofa
{

namespace gui
{

namespace qt
{

//enum TYPE{ NORMAL, PML, LML};
enum SCRIPT_TYPE { PHP, PERL };

enum
{
    ALL,
    VISUALMODELS,
    BEHAVIORMODELS,
    COLLISIONMODELS,
    BOUNDINGTREES,
    MAPPINGS,
    MECHANICALMAPPINGS,
    FORCEFIELDS,
    INTERACTIONS,
    WIREFRAME,
    NORMALS
};

using sofa::simulation::tree::GNode;
using sofa::simulation::Node;
#ifdef SOFA_PML
using namespace sofa::filemanager::pml;
#endif


class RealGUI : public ::GUI, public SofaGUI
{
    Q_OBJECT

    /// @name SofaGUI Interface
    /// @{


public:

    static int InitGUI(const char* name, const std::vector<std::string>& options);
    static SofaGUI* CreateGUI(const char* name, const std::vector<std::string>& options, sofa::simulation::Node* groot = NULL, const char* filename = NULL);

    int mainLoop();

    int closeGUI();

    Node* currentSimulation();

    /// @}

    const char* viewerName;

    sofa::gui::qt::viewer::SofaViewer* viewer;

    RealGUI( const char* viewername, const std::vector<std::string>& options = std::vector<std::string>() );
    ~RealGUI();


    virtual void fileOpen(std::string filename); //, int TYPE=NORMAL);
    virtual void fileOpenSimu(std::string filename); //, int TYPE=NORMAL);
    virtual void setScene(Node* groot, const char* filename=NULL);
    virtual void setTitle( std::string windowTitle );

    //public slots:
    virtual void fileNew();
    virtual void fileOpen();
    virtual void fileSave();
    virtual void fileSaveAs() {fileSaveAs((Node *)NULL);};
    virtual void fileSaveAs(Node *node);
    virtual void fileSaveAs(Node* node,const char* filename);

    virtual void fileReload();
    virtual void fileExit();
    virtual void saveXML();
    //virtual void editUndo();
    //virtual void editRedo();
    //virtual void editCut();
    //virtual void editCopy();
    //virtual void editPaste();
    //virtual void editFind();
    virtual void viewerOpenGL();
    virtual void viewerQGLViewer();
    virtual void viewerOGRE();
    virtual void viewExecutionGraph();
    //virtual void helpIndex();
    //virtual void helpContents();
    //virtual void helpAbout();

    virtual void editRecordDirectory();
    virtual void editGnuplotDirectory();

    void dragEnterEvent( QDragEnterEvent* event) {event->accept();}
    void dropEvent(QDropEvent* event);

public slots:
    void fileRecentlyOpened(int id);
    void updateRecentlyOpened(std::string fileLoaded);
    void DoubleClickeItemInSceneView(QListViewItem * item);
    void RightClickedItemInSceneView(QListViewItem *item, const QPoint& point, int index);
    void playpauseGUI(bool value);
    void step();
    void animate();
    void setDt(double);
    void setDt(const QString&);
    void resetScene();
    void screenshot();

#ifdef SOFA_QT4
    void flagChanged(Q3ListViewItem *item)
    {

        if ( dynamic_cast<DisplayFlagItem *>(item) )
        {
            DisplayFlagItem *flag=dynamic_cast<DisplayFlagItem *>(item);

            flag->switchState();
        }
        else if (dynamic_cast<Q3CheckListItem*>(item))
        {
            Q3CheckListItem* checkItem=dynamic_cast<Q3CheckListItem*>(item);

            if (checkItem->isOn()) checkItem->setState(Q3CheckListItem::Off);
            else checkItem->setState(Q3CheckListItem::On);
        }
    };

    void flagDoubleClicked(Q3ListViewItem *item)
    {
        item->setOpen ( !item->isOpen() );
        flagChanged(item);
    }
#else
    void flagChanged(QListViewItem *item)
    {

        if ( dynamic_cast<DisplayFlagItem *>(item) )
        {
            DisplayFlagItem *flag=dynamic_cast<DisplayFlagItem *>(item);

            flag->switchState();
        }
        else if (dynamic_cast<Q3CheckListItem*>(item))
        {
            Q3CheckListItem* checkItem=dynamic_cast<Q3CheckListItem*>(item);

            if (checkItem->isOn()) checkItem->setState(Q3CheckListItem::Off);
            else checkItem->setState(Q3CheckListItem::On);
        }
    };

    void flagDoubleClicked(QListViewItem *item)
    {
        item->setOpen ( !item->isOpen() );
        flagChanged(item);
    }
#endif

    void showVisualModels()      {showhideElements(VISUALMODELS,true);};
    void showBehaviorModels()    {showhideElements(BEHAVIORMODELS,true);};
    void showCollisionModels()   {showhideElements(COLLISIONMODELS,true);};
    void showBoundingTrees()     {showhideElements(BOUNDINGTREES,true);};
    void showMappings()          {showhideElements(MAPPINGS,true);};
    void showMechanicalMappings() {showhideElements(MECHANICALMAPPINGS,true);};
    void showForceFields()       {showhideElements(FORCEFIELDS,true);};
    void showInteractions()      {showhideElements(INTERACTIONS,true);};
    void showAll()               {showhideElements(ALL,true);};
    void showWireFrame()         {showhideElements(WIREFRAME,true);};
    void showNormals()           {showhideElements(NORMALS,true);};

    void hideVisualModels()      {showhideElements(VISUALMODELS,false);};
    void hideBehaviorModels()    {showhideElements(BEHAVIORMODELS,false);};
    void hideCollisionModels()   {showhideElements(COLLISIONMODELS,false);};
    void hideBoundingTrees()     {showhideElements(BOUNDINGTREES,false);};
    void hideMappings()          {showhideElements(MAPPINGS,false);};
    void hideMechanicalMappings() {showhideElements(MECHANICALMAPPINGS,false);};
    void hideForceFields()       {showhideElements(FORCEFIELDS,false);};
    void hideInteractions()      {showhideElements(INTERACTIONS,false);};
    void hideAll()               {showhideElements(ALL,false);};
    void hideWireFrame()         {showhideElements(WIREFRAME,false);};
    void hideNormals()           {showhideElements(NORMALS,false);};

    void showhideElements(int FILTER, bool value);

    void clearRecord();
    void slot_recordSimulation( bool);
    void slot_backward( );
    void slot_stepbackward( );
// 	  void slot_playbackward(  );
    void slot_playforward(  ) ;
    void slot_stepforward( ) ;
    void slot_forward( );
    void slot_sliderValue(int);
    void slot_loadrecord_timevalue();



    void changeInstrument(int);

    void clearGraph();
    //Used in Context Menu
    void graphSaveObject();
    void graphAddObject();
    void graphRemoveObject();
    void graphModify();
    void graphCollapse();
    void graphExpand();
    void graphDesactivateNode();
    void graphActivateNode();
    //When adding an object in the graph
    void loadObject(std::string path, double dx, double dy, double dz,double rx, double ry, double rz, double scale=1.0);
    //refresh the visualization window
    void redraw();
    //when a dialog modify object is closed
    void modifyUnlock(void *Id);
    void transformObject( Node *node, double dx, double dy, double dz, double rx, double ry, double rz, double scale=1.0);


    void exportGraph();
    void exportGraph(sofa::simulation::tree::GNode*);
    void exportOBJ(bool exportMTL=true);
    void dumpState(bool);
    void displayComputationTime(bool);
    void setExportGnuplot(bool);
    void currentTabChanged(QWidget*);

signals:
    void reload();
    void newScene();
    void newStep();
    void quit();

protected:

    void eventNewStep();
    void eventNewTime();
    void init();
    void keyPressEvent ( QKeyEvent * e );

    void loadSimulation(bool one_step=false);

    void initDesactivatedNode();
    //Graph Stats
    bool graphCreateStats(Node *groot);
    void graphAddCollisionModelsStat(sofa::helper::vector< sofa::core::CollisionModel* > &v);
    void graphSummary();

    bool isErasable(core::objectmodel::Base* element);

    bool m_dumpState;
    std::ofstream* m_dumpStateStream;
    bool m_exportGnuplot;
    bool _animationOBJ; int _animationOBJcounter;// save a succession of .obj indexed by _animationOBJcounter
    bool m_displayComputationTime;


    QWidget* currentTab;
    QWidget *tabInstrument;

    GraphListenerQListView* graphListener;
    QListViewItem *item_clicked;
    GNode *node_clicked;
    QTimer* timerStep;
    QTimer* timerRecordStep;
    QLabel* fpsLabel;
    QLabel* timeLabel;

    void setPixmap(std::string pixmap_filename, QPushButton* b);

    double getRecordInitialTime() const;
    void   setRecordInitialTime(const double time);
    double getRecordFinalTime  () const;
    void   setRecordFinalTime  (const double time);
    double getRecordTime       () const;
    void   setRecordTime       (const double time);
    void   setTimeSimulation   (const double time);

    QPushButton* record;
    QPushButton* backward_record;
    QPushButton* stepbackward_record;
// 	  QPushButton* playbackward_record;
    QPushButton* playforward_record;
    QPushButton* stepforward_record;
    QPushButton* forward_record;

    QLineEdit* loadRecordTime;
    QLabel* initialTime;
    QLabel* finalTime;
    QSlider* timeSlider;

    std::string simulation_name;
    std::string record_directory;
    std::string gnuplot_directory;
    std::string writeSceneName;

    QWidgetStack* left_stack;
    AddObject *dialog;
    QPopupMenu *recentlyOpened;


    sofa::simulation::tree::GNode* getScene() { if (viewer) return viewer->getScene(); else return NULL; }

    void sleep(unsigned int mseconds, unsigned int init_time)
    {
        unsigned int t = 0;
        clock_t goal = mseconds + init_time;
        while (goal > clock()) t++;
    }

private:
    //Map: Id -> Node currently modified. Used to avoid dependancies during removing actions
    std::map< void*, core::objectmodel::Base* >            map_modifyDialogOpened;

    std::map< void*, QDialog* >                       map_modifyObjectWindow;
    std::vector<std::pair<core::objectmodel::Base*, Q3ListViewItem*> > items_stats;
    //unique ID to pass to a modify object dialog
    void *current_Id_modifyDialog;


    //currently unused: scale is experimental
    float object_Scale[2];

    float initial_time;
    int frameCounter;
    //At initialization: list of the path to the basic objects you can add to the scene
    std::vector< std::string > list_object;
    std::list< GNode *> list_object_added;
    std::list< GNode *> list_object_removed;
    std::list< GNode *> list_object_initial;
    bool record_simulation;

    bool setViewer(const char* name);
    void addViewer();
    void setGUI(void);

    bool setWriteSceneName();
    void addReadState(bool init);
    void addWriteState();

#ifdef SOFA_PML
    virtual void pmlOpen(const char* filename, bool resetView=true);
    virtual void lmlOpen(const char* filename);
    PMLReader *pmlreader;
    LMLReader *lmlreader;
#endif


    class DisplayFlagItem : public Q3CheckListItem
    {
    protected:
        RealGUI* gui;
        int id;
        ToggleState last;
    public:
        template<class T>
        DisplayFlagItem(RealGUI* g, T* parent, int id, const QString & text, Type tt = CheckBox)
            : Q3CheckListItem(parent, text, tt)
            , gui(g)
            , id(id)
            , last(Off)
        {
            if (tt == CheckBoxController)
                setTristate(true);
            //setState(NoChange);

        }
        template<class T>
        DisplayFlagItem(RealGUI* g, T* parent, Q3CheckListItem* after, int id, const QString & text, Type tt = CheckBox)
            : Q3CheckListItem(parent, after, text, tt)
            , gui(g)
            , id(id)
            , last(Off)
        {
            if (tt == CheckBoxController)
                setTristate(true);
            //setState(NoChange);
        }
        void setState( ToggleState s )
        {
            last = s;
            Q3CheckListItem::setState( s );
        }
        void init( bool b )
        {
            setState( b ? On : Off );
        }
        void switchState()
        {
            if (isOn()) {setState(Off); gui->showhideElements(id,false);}
            else        {setState(On); gui->showhideElements(id,true);}
        }
    protected:
        virtual void stateChange ( bool b )
        {
            ToggleState s = state();
            if (s == last) return;
            if (s == NoChange) return;
            last = s;
            gui->showhideElements(id,b);
        }

    };


    DisplayFlagItem* itemShowAll;
    DisplayFlagItem* itemShowVisualModels;
    DisplayFlagItem* itemShowBehaviorModels;
    DisplayFlagItem* itemShowCollisionModels;
    DisplayFlagItem* itemShowBoundingTrees;
    DisplayFlagItem* itemShowMappings;
    DisplayFlagItem* itemShowMechanicalMappings;
    DisplayFlagItem* itemShowForceFields;
    DisplayFlagItem* itemShowInteractions;
    DisplayFlagItem* itemShowWireFrame;
    DisplayFlagItem* itemShowNormals;

};

} // namespace qt

} // namespace gui

} // namespace sofa

#endif // SOFA_GUI_VIEWER_REALGUI_H
