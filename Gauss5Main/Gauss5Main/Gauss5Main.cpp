#include <wx/wx.h>
#include <iostream>
#include "transformationCPP.h"
#include <fstream>
#include <vector>
#include <future>
#include <thread>
#include <random>
#include "Bitmap.h"
#include "timer.h"

extern "C" int basicTrfAsm(uint8_t* beginPtr, uint8_t * endPtr);
extern "C" int gaussTrfAsm(uint8_t * beginPtr, uint8_t * endPtr, int width);

double filterApplication(wxString path, int threads, bool Asm) {
    BMP bitmap(path);
    //int threads = 16;
    double time = 0;
    //bool Asm = true;
    //BMP test("t2_24.bmp");
    //BMP testC("t2_24.bmp");
    //while (test.data.size() % threads != 0) threads--; //Pętla obcinająca ilość wątków do takiej, której uda się równo obsłużyć
    int overflow = (bitmap.data_size / 3) % threads;
    int usedOverflow = 0;
    int overflowC = (bitmap.data_size / 3) % threads;
    int usedOverflowC = 0;
    std::vector<std::thread> handler;
    std::vector<std::thread> handlerC;
    //if (overflow != 0) std::cout << "Nierowne thready\n";
    if (!Asm) {
        Timer::start();
        for (int i = 0; i < threads; i++) {
            if (overflowC != 0) {
                handlerC.push_back(std::thread(gaussTrf, bitmap.beginData + i * bitmap.data_size / threads + (usedOverflowC * 3)-6, (bitmap.beginData + (i + 1) * bitmap.data_size / threads) + 2 + (usedOverflowC * 3)-6, bitmap.bmp_info_header.width + 4));

                overflowC--;
                usedOverflowC++;
            }
            else {
                handlerC.push_back(std::thread(gaussTrf, bitmap.beginData + i * bitmap.data_size / threads + (usedOverflowC * 3)-6, (bitmap.beginData + (i + 1) * bitmap.data_size / threads) - 1 + (usedOverflowC * 3)-6, bitmap.bmp_info_header.width + 4));
            }
        }
        for (int i = 0; i < threads; i++) {
            handlerC[i].join();
        }
        //gaussTrfAsm(test.beginData, test.beginData + test.data_size, test.bmp_info_header.width);
        time = Timer::stop();
    }
    else {
        Timer::start();
        for (int i = 0; i < threads; i++) {
            if (overflow != 0) {
                handler.push_back(std::thread(gaussTrfAsm, bitmap.beginData + i * bitmap.data_size / threads + (usedOverflow * 3), (bitmap.beginData + (i + 1) * bitmap.data_size / threads) + 2 + (usedOverflow * 3) + 1, bitmap.bmp_info_header.width + 4));
                overflow--;
                usedOverflow++;
            }
            else {
                handler.push_back(std::thread(gaussTrfAsm, bitmap.beginData + i * bitmap.data_size / threads + (usedOverflow * 3), (bitmap.beginData + (i + 1) * bitmap.data_size / threads) - 1 + (usedOverflow * 3) + 1, bitmap.bmp_info_header.width + 4));
            }
        }
        for (int i = 0; i < threads; i++) {
            handler[i].join();
        }
        //basicTrf(testC.data.data(), testC.data.data() + testC.data.size() - 1);
        time = Timer::stop();
    }

    //if (overflowC != 0) std::cout << "Nierowne thready\n";


    bitmap.write("kopia.bmp");
    return time;
    //testC.write("kopiaC.bmp");
}
 
class MyApp : public wxApp
{
public:
    bool OnInit() override;
};

wxIMPLEMENT_APP(MyApp);

class wxImagePanel : public wxPanel
{
    wxImage image;
    wxBitmap resized;
    int w, h;

public:
    wxImagePanel(wxFrame* parent, wxString file, wxBitmapType format);

    void paintEvent(wxPaintEvent& evt);
    void paintNow();
    void OnSize(wxSizeEvent& event);
    void render(wxDC& dc);

    // some useful events
    /*
     void mouseMoved(wxMouseEvent& event);
     void mouseDown(wxMouseEvent& event);
     void mouseWheelMoved(wxMouseEvent& event);
     void mouseReleased(wxMouseEvent& event);
     void rightClick(wxMouseEvent& event);
     void mouseLeftWindow(wxMouseEvent& event);
     void keyPressed(wxKeyEvent& event);
     void keyReleased(wxKeyEvent& event);
     */

    DECLARE_EVENT_TABLE()
};


BEGIN_EVENT_TABLE(wxImagePanel, wxPanel)
// some useful events
/*
 EVT_MOTION(wxImagePanel::mouseMoved)
 EVT_LEFT_DOWN(wxImagePanel::mouseDown)
 EVT_LEFT_UP(wxImagePanel::mouseReleased)
 EVT_RIGHT_DOWN(wxImagePanel::rightClick)
 EVT_LEAVE_WINDOW(wxImagePanel::mouseLeftWindow)
 EVT_KEY_DOWN(wxImagePanel::keyPressed)
 EVT_KEY_UP(wxImagePanel::keyReleased)
 EVT_MOUSEWHEEL(wxImagePanel::mouseWheelMoved)
 */

 // catch paint events
    EVT_PAINT(wxImagePanel::paintEvent)
    //Size event
    EVT_SIZE(wxImagePanel::OnSize)
    END_EVENT_TABLE()


    // some useful events
    /*
     void wxImagePanel::mouseMoved(wxMouseEvent& event) {}
     void wxImagePanel::mouseDown(wxMouseEvent& event) {}
     void wxImagePanel::mouseWheelMoved(wxMouseEvent& event) {}
     void wxImagePanel::mouseReleased(wxMouseEvent& event) {}
     void wxImagePanel::rightClick(wxMouseEvent& event) {}
     void wxImagePanel::mouseLeftWindow(wxMouseEvent& event) {}
     void wxImagePanel::keyPressed(wxKeyEvent& event) {}
     void wxImagePanel::keyReleased(wxKeyEvent& event) {}
     */

    wxImagePanel::wxImagePanel(wxFrame* parent, wxString file, wxBitmapType format) :
    wxPanel(parent)
{
    // load the file... ideally add a check to see if loading was successful
    image.LoadFile(file, format);
    w = -1;
    h = -1;
}

/*
 * Called by the system of by wxWidgets when the panel needs
 * to be redrawn. You can also trigger this call by
 * calling Refresh()/Update().
 */

void wxImagePanel::paintEvent(wxPaintEvent& evt)
{
    // depending on your system you may need to look at double-buffered dcs
    wxPaintDC dc(this);
    render(dc);
}

/*
 * Alternatively, you can use a clientDC to paint on the panel
 * at any time. Using this generally does not free you from
 * catching paint events, since it is possible that e.g. the window
 * manager throws away your drawing when the window comes to the
 * background, and expects you will redraw it when the window comes
 * back (by sending a paint event).
 */
void wxImagePanel::paintNow()
{
    // depending on your system you may need to look at double-buffered dcs
    wxClientDC dc(this);
    render(dc);
}

/*
 * Here we do the actual rendering. I put it in a separate
 * method so that it can work no matter what type of DC
 * (e.g. wxPaintDC or wxClientDC) is used.
 */
void wxImagePanel::render(wxDC& dc)
{
    int neww, newh;
    dc.GetSize(&neww, &newh);

    if (neww != w || newh != h)
    {
        resized = wxBitmap(image.Scale(neww, newh /*, wxIMAGE_QUALITY_HIGH*/));
        w = neww;
        h = newh;
        dc.DrawBitmap(resized, 0, 0, false);
    }
    else {
        dc.DrawBitmap(resized, 0, 0, false);
    }
}

/*
 * Here we call refresh to tell the panel to draw itself again.
 * So when the user resizes the image panel the image should be resized too.
 */
void wxImagePanel::OnSize(wxSizeEvent& event) {
    Refresh();
    //skip the event.
    event.Skip();
}

class MyFrame : public wxFrame
{
public:
    MyFrame();
    wxButton* applyButton;
    wxRadioButton* asmButton;
    wxRadioButton* cppButton;
    wxGridSizer* topBox;
    wxGridSizer* leftBox;
    wxBoxSizer* sizerL = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizerR = new wxBoxSizer(wxHORIZONTAL);
    wxGridSizer* buttonBox;
    wxImagePanel* before;
    wxImagePanel* after;
    wxSlider* threads;
    wxStaticText* tDisplay;

private:
    void OnLoad(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnApply(wxCommandEvent& event);
    void OnChange(wxCommandEvent& event);
    wxString path;
};

enum
{
    ID_Load = 1,
    ID_Apply = 2,
    ID_Asm = 3,
    ID_Cpp = 4,
    ID_Threads = 5,
    ID_Dthreads = 6
};

bool MyApp::OnInit()
{
    MyFrame* frame = new MyFrame();
    frame->Show(true);
    return true;
}

MyFrame::MyFrame()
    : wxFrame(nullptr, wxID_ANY, "Gaussian Filter")
{
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(ID_Load, "&Load bitmap...\tCtrl-H",
        "");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");

    asmButton = new wxRadioButton(this, ID_Asm, _("Asm"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP, wxDefaultValidator, _("asmButton"));
    cppButton = new wxRadioButton(this, ID_Asm, _("C++"), wxDefaultPosition, wxDefaultSize, 0L, wxDefaultValidator, _("cppButton"));
    applyButton = new wxButton(this, ID_Apply, _("Apply Filter"), wxDefaultPosition, wxDefaultSize, 0L, wxDefaultValidator, _("applyButton"));
    threads = new wxSlider(this, ID_Threads, 2, 1, 64, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL, wxDefaultValidator, _("threadsSlider"));
    tDisplay = new wxStaticText(this, ID_Dthreads, wxString::Format(wxT("%d"), (int)threads->GetValue()), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
    

    SetMenuBar(menuBar);

    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");

    topBox = new wxGridSizer(1, 3, 3, 3);
    leftBox = new wxGridSizer(6, 1, 3, 3);
    buttonBox = new wxGridSizer(1, 2, 3, 3);
    leftBox->Add(new wxStaticText(this, -1, wxT("")), 0, wxEXPAND);
    buttonBox->Add(asmButton, 1, wxALIGN_CENTER);
    buttonBox->Add(cppButton, 1, wxALIGN_CENTER);
    leftBox->Add(buttonBox, 1, wxEXPAND);
    leftBox->Add(new wxStaticText(this, -1, wxT("")), 0, wxEXPAND);
    leftBox->Add(threads, 0, wxEXPAND, wxALIGN_CENTER);
    leftBox->Add(tDisplay, 0, wxEXPAND, wxALIGN_CENTER);
    leftBox->Add(applyButton, 1, wxALIGN_CENTER);
    Bind(wxEVT_MENU, &MyFrame::OnLoad, this, ID_Load);
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_BUTTON, &MyFrame::OnApply, this, ID_Apply);
    Bind(wxEVT_SLIDER, &MyFrame::OnChange, this, ID_Threads);

    topBox->Add(leftBox, 1, wxEXPAND);
    topBox->Add(sizerL, 1, wxEXPAND);
    topBox->Add(sizerR, 1, wxEXPAND);
    SetSizer(topBox);
}

void MyFrame::OnChange(wxCommandEvent& event) {
    tDisplay->SetLabel(wxString::Format(wxT("%d"), (int)threads->GetValue()));
}

void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets Hello World example",
        "About Hello World", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnLoad(wxCommandEvent& event)
{
    Refresh();
    delete before;
    wxFileDialog
        openFileDialog(this, _("Open BMP file"), "", "",
            "BMP files (*.bmp)|*.bmp", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    path = openFileDialog.GetFilename();
    before = new wxImagePanel(this, openFileDialog.GetFilename(), wxBITMAP_TYPE_BMP);
    //before = new wxStaticBitmap(this, wxID_ANY, wxBitmap(openFileDialog.GetFilename(), wxBITMAP_TYPE_BMP), wxPoint(50, 100), wxSize(100, 500));
    sizerL->Add(before, 1, wxEXPAND);
    Layout();
    //Refresh();
}

void MyFrame::OnApply(wxCommandEvent& event) {
    delete after;
    double time = filterApplication(path, threads->GetValue(), asmButton->GetValue());
    std::string result = "Time" + std::to_string(time) + "ms";
    wxMessageBox(_(result), "result", wxOK | wxICON_INFORMATION);
    after = new wxImagePanel(this, "kopia.bmp", wxBITMAP_TYPE_BMP);
    sizerR->Add(after, 1, wxEXPAND);
    Layout();
}

int main()
{
    /*
    int threads = 16;

    bool Asm = false;
    BMP test("t2_24.bmp");
    BMP testC("t2_24.bmp");
    //while (test.data.size() % threads != 0) threads--; //Pętla obcinająca ilość wątków do takiej, której uda się równo obsłużyć
    int overflow = (test.data_size/3) % threads;
    int usedOverflow = 0;
    int overflowC = (testC.data_size/3) % threads;
    int usedOverflowC = 0;
    std::vector<std::thread> handler;
    std::vector<std::thread> handlerC;
    //if (overflow != 0) std::cout << "Nierowne thready\n";
    if(!Asm){
        Timer::start();
        for (int i = 0; i < threads; i++) {
            if (overflowC != 0) {
                handlerC.push_back(std::thread(gaussTrf, testC.beginData + i * testC.data_size / threads + (usedOverflowC * 3), (testC.beginData + (i + 1) * testC.data_size / threads) + 2 + (usedOverflowC * 3), testC.bmp_info_header.width + 4));

                overflowC--;
                usedOverflowC++;
            }
            else {
                handlerC.push_back(std::thread(gaussTrf, testC.beginData + i * testC.data_size / threads + (usedOverflowC * 3), (testC.beginData + (i + 1) * testC.data_size / threads) - 1 + (usedOverflowC * 3), testC.bmp_info_header.width + 4));
            }
        }
        for (int i = 0; i < threads; i++) {
            handlerC[i].join();
        }
        //gaussTrfAsm(test.beginData, test.beginData + test.data_size, test.bmp_info_header.width);
        Timer::stop();
    }
    else {
        Timer::start();
        for (int i = 0; i < threads; i++) {
            if (overflow != 0) {
                handler.push_back(std::thread(gaussTrfAsm, test.beginData + i * test.data_size / threads + (usedOverflow * 3), (test.beginData + (i + 1) * test.data_size / threads) + 2 + (usedOverflow * 3) + 1, test.bmp_info_header.width + 4));
                overflow--;
                usedOverflow++;
            }
            else {
                handler.push_back(std::thread(gaussTrfAsm, test.beginData + i * test.data_size / threads + (usedOverflow * 3), (test.beginData + (i + 1) * test.data_size / threads) - 1 + (usedOverflow * 3) + 1, test.bmp_info_header.width + 4));
            }
        }
        for (int i = 0; i < threads; i++) {
            handler[i].join();
        }
        //basicTrf(testC.data.data(), testC.data.data() + testC.data.size() - 1);
        Timer::stop();
    }
    
    //if (overflowC != 0) std::cout << "Nierowne thready\n";
    
    */
    //test.write("kopia.bmp");
    //testC.write("kopiaC.bmp");
}
