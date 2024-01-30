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
extern "C" int gaussTrfAsm(uint8_t * beginPtr, uint8_t * endPtr, uint8_t* copyBegin, int width);
extern "C" int badAsm(uint8_t * beginPtr, uint8_t * endPtr, uint8_t * copyBegin, int width);
extern "C" int horizontalAsm(uint8_t * beginPtr, uint8_t * endPtr, uint8_t * copyBegin, int width);
extern "C" int verticalAsm(uint8_t * beginPtr, uint8_t * endPtr, uint8_t * copyBegin, int width);

double overheadFunc() {
    return 1;
}

double overheadTest(int threads) {
    Timer::start();
    double time = 0;
    std::vector<std::thread> handler;
    for (int i = 0; i < threads; i++) {
        handler.emplace_back(overheadFunc);
    }
    for (int i = 0; i < threads; i++) {
        handler[i].join();
    }
    time = Timer::stop();
}

double filterApplication(wxString path, int threads, bool Asm) {
    double time = 0;
    BMP bitmap(path);
    BMP blurred(path);
    int processSize = 3;
    int chunkNum = ceil(bitmap.data_size / processSize); //Amount of 5-pixel (or less - if the amount of pixels isnt divisible by 5 the tail of the image is still considered a full chunk) chunks in the image
    int overflow = chunkNum % threads; //How many threads will have to take an extra chunk
    int baseSlice = chunkNum * processSize / threads; //Base amount of data each thread processes
    int usedOverflow = 0;
    int overflowC = bitmap.data_size % threads;
    int usedOverflowC = 0;
    std::vector<std::thread> handler;
    std::vector<std::thread> handlerC;
    if (!Asm) {
        Timer::start();
        for (int i = 0; i < threads; i++) {
            if (overflowC != 0) {
                handlerC.push_back(std::thread(gaussTrf, bitmap.beginData + i * bitmap.data_size / threads + usedOverflowC, (bitmap.beginData + (i + 1) * bitmap.data_size / threads) + 1 + usedOverflowC, blurred.beginData + i * bitmap.data_size / threads + usedOverflowC, bitmap.bmp_info_header.width + 4));

                overflowC--;
                usedOverflowC++;
            }
            else {
                handlerC.push_back(std::thread(gaussTrf, bitmap.beginData + i * bitmap.data_size / threads + usedOverflowC, (bitmap.beginData + (i + 1) * bitmap.data_size / threads) + usedOverflowC, blurred.beginData + i * blurred.data_size / threads + usedOverflowC, bitmap.bmp_info_header.width + 4));
            }
        }
        for (int i = 0; i < threads; i++) {
            handlerC[i].join();
        }
        time = Timer::stop();
    }
    else {
        Timer::start();
        for (int i = 0; i < threads; i++) {
            uint8_t* begin = bitmap.beginData + i * baseSlice + usedOverflow; //Start address for the thread
            uint8_t* end = bitmap.beginData + (i + 1) * baseSlice + usedOverflow;
            uint8_t* bufferBegin = blurred.beginData + i * baseSlice + usedOverflow;
            if (overflow != 0) {
                handler.push_back(std::thread(badAsm, begin, end + processSize, bufferBegin, bitmap.bmp_info_header.width + 4));
                overflow--;
                usedOverflow+=processSize;
            }
            else {
                handler.push_back(std::thread(badAsm, begin, end, bufferBegin, bitmap.bmp_info_header.width + 4));
            }
        }
        for (int i = 0; i < handler.size(); i++) {
            handler[i].join();
        }
        time = Timer::stop();
    }
    bitmap.write("kopia.bmp");
    return time;
}
double statisticalTest(wxString path, int threads, bool Asm) {
    double time = 0;
    for (int i = 0; i < 10; i++) {
        BMP bitmap(path);
        BMP blurred(path);
        int processSize = 16;
        int chunkNum = ceil(bitmap.data_size / processSize); //Amount of 5-pixel (or less - if the amount of pixels isnt divisible by 5 the tail of the image is still considered a full chunk) chunks in the image
        int overflow = chunkNum % threads; //How many threads will have to take an extra chunk
        int baseSlice = chunkNum * processSize / threads; //Base amount of data each thread processes
        int usedOverflow = 0;
        int overflowC = bitmap.data_size % threads;
        int usedOverflowC = 0;
        std::vector<std::thread> handler;
        std::vector<std::thread> handlerC;
        if (!Asm) {
            Timer::start();
            for (int i = 0; i < threads; i++) {
                if (overflowC != 0) {
                    handlerC.push_back(std::thread(gaussTrf, bitmap.beginData + i * bitmap.data_size / threads + usedOverflowC, (bitmap.beginData + (i + 1) * bitmap.data_size / threads) + 1 + usedOverflowC, blurred.beginData + i * bitmap.data_size / threads + usedOverflowC, bitmap.bmp_info_header.width + 4));

                    overflowC--;
                    usedOverflowC++;
                }
                else {
                    handlerC.push_back(std::thread(gaussTrf, bitmap.beginData + i * bitmap.data_size / threads + usedOverflowC, (bitmap.beginData + (i + 1) * bitmap.data_size / threads) + usedOverflowC, blurred.beginData + i * blurred.data_size / threads + usedOverflowC, bitmap.bmp_info_header.width + 4));
                }
            }
            for (int i = 0; i < threads; i++) {
                handlerC[i].join();
            }
            time += Timer::stop();
        }
        else {
            Timer::start();
            for (int i = 0; i < threads; i++) {
                uint8_t* begin = bitmap.beginData + i * baseSlice + usedOverflow; //Start address for the thread
                uint8_t* end = bitmap.beginData + (i + 1) * baseSlice + usedOverflow;
                uint8_t* bufferBegin = blurred.beginData + i * baseSlice + usedOverflow;
                if (overflow != 0) {
                    handler.push_back(std::thread(gaussTrfAsm, begin, end + processSize, bufferBegin, bitmap.bmp_info_header.width + 4));
                    overflow--;
                    usedOverflow += processSize;
                }
                else {
                    handler.push_back(std::thread(gaussTrfAsm, begin, end, bufferBegin, bitmap.bmp_info_header.width + 4));
                }
            }
            for (int i = 0; i < handler.size(); i++) {
                handler[i].join();
            }
            time += Timer::stop();
        }
    }
    return time/10;
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

    DECLARE_EVENT_TABLE()
};


BEGIN_EVENT_TABLE(wxImagePanel, wxPanel)

 // catch paint events
    EVT_PAINT(wxImagePanel::paintEvent)
    //Size event
    EVT_SIZE(wxImagePanel::OnSize)
    END_EVENT_TABLE()

    wxImagePanel::wxImagePanel(wxFrame* parent, wxString file, wxBitmapType format) :
    wxPanel(parent)
{
    // load the file... ideally add a check to see if loading was successful
    image.LoadFile(file, format);
    w = -1;
    h = -1;
}


void wxImagePanel::paintEvent(wxPaintEvent& evt)
{
    // depending on your system you may need to look at double-buffered dcs
    wxPaintDC dc(this);
    render(dc);
}

void wxImagePanel::paintNow()
{
    // depending on your system you may need to look at double-buffered dcs
    wxClientDC dc(this);
    render(dc);
}

void wxImagePanel::render(wxDC& dc)
{
    int neww, newh;
    dc.GetSize(&neww, &newh);

    if (neww != w || newh != h)
    {
        resized = wxBitmap(image.Scale(neww, newh));
        w = neww;
        h = newh;
        dc.DrawBitmap(resized, 0, 0, false);
    }
    else {
        dc.DrawBitmap(resized, 0, 0, false);
    }
}

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
    wxButton* testButton;
    wxRadioButton* asmButton;
    wxRadioButton* cppButton;
    wxGridSizer* topBox;
    wxGridSizer* leftBox;
    wxBoxSizer* sizerL = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizerR = new wxBoxSizer(wxHORIZONTAL);
    wxGridSizer* buttonBox;
    wxGridSizer* buttonBox2;
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
    void OnTest(wxCommandEvent& event);
    wxString path;
};

enum
{
    ID_Load = 1,
    ID_Apply = 2,
    ID_Asm = 3,
    ID_Cpp = 4,
    ID_Threads = 5,
    ID_Dthreads = 6,
    ID_Test = 7
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
    testButton = new wxButton(this, ID_Test, _("Test Performance"), wxDefaultPosition, wxDefaultSize, 0L, wxDefaultValidator, _("testButton"));
    threads = new wxSlider(this, ID_Threads, 2, 1, 64, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL, wxDefaultValidator, _("threadsSlider"));
    tDisplay = new wxStaticText(this, ID_Dthreads, wxString::Format(wxT("%d"), (int)threads->GetValue()), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
    

    SetMenuBar(menuBar);

    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");

    topBox = new wxGridSizer(1, 3, 3, 3);
    leftBox = new wxGridSizer(6, 1, 3, 3);
    buttonBox = new wxGridSizer(1, 2, 3, 3);
    buttonBox2 = new wxGridSizer(1, 2, 3, 3);
    leftBox->Add(new wxStaticText(this, -1, wxT("")), 0, wxEXPAND);
    buttonBox->Add(asmButton, 1, wxALIGN_CENTER);
    buttonBox->Add(cppButton, 1, wxALIGN_CENTER);
    leftBox->Add(buttonBox, 1, wxEXPAND);
    leftBox->Add(new wxStaticText(this, -1, wxT("")), 0, wxEXPAND);
    leftBox->Add(threads, 0, wxEXPAND, wxALIGN_CENTER);
    leftBox->Add(tDisplay, 0, wxEXPAND, wxALIGN_CENTER);
    buttonBox2->Add(applyButton, 1, wxALIGN_CENTER);
    buttonBox2->Add(testButton, 1, wxALIGN_CENTER);
    leftBox->Add(buttonBox2, 1, wxEXPAND);
    Bind(wxEVT_MENU, &MyFrame::OnLoad, this, ID_Load);
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_BUTTON, &MyFrame::OnApply, this, ID_Apply);
    Bind(wxEVT_BUTTON, &MyFrame::OnTest, this, ID_Test);
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

void MyFrame::OnTest(wxCommandEvent& event) {
    double time = statisticalTest(path, threads->GetValue(), asmButton->GetValue());
    //double time = overheadTest(threads->GetValue());
    std::string result = "Average time for 10 filter applications: " + std::to_string(time) + "ms";
    wxMessageBox(_(result), "result", wxOK | wxICON_INFORMATION);
}

int main()
{
    
    int threads = 4;
    BMP bitmap("test4.bmp");
    BMP blurred("test4.bmp");
    //BMP bitmap("t2_24.bmp");
    //BMP blurred("t2_24.bmp");
    //std::vector<uint8_t> blurred = bitmap.data;
    bool Asm = true;
    double time = 0;
    double processSize = 16;
    double temp = bitmap.data_size / processSize;
    int chunkNum = ceil(temp); //Amount of 5-pixel (or less - if the amount of pixels isnt divisible by 5 the tail of the image is still considered a full chunk) chunks in the image
    int overflow = chunkNum % threads; //How many threads will have to take an extra chunk
    int baseSlice = chunkNum * processSize / threads; //Base amount of data each thread processes
    int usedOverflow = 0;
    int overflowC = bitmap.data_size % threads;
    int usedOverflowC = 0;
    std::vector<std::thread> handler;
    std::vector<std::thread> handlerC;
    if (!Asm) {
        Timer::start();
        for (int i = 0; i < threads; i++) {
            if (overflowC != 0) {
                handlerC.push_back(std::thread(gaussTrf, bitmap.beginData + i * bitmap.data_size / threads + usedOverflowC, (bitmap.beginData + (i + 1) * bitmap.data_size / threads) + 1 + usedOverflowC, blurred.beginData + i * bitmap.data_size / threads + usedOverflowC, bitmap.bmp_info_header.width + 4));

                overflowC--;
                usedOverflowC++;
            }
            else {
                handlerC.push_back(std::thread(gaussTrf, bitmap.beginData + i * bitmap.data_size / threads + usedOverflowC, (bitmap.beginData + (i + 1) * bitmap.data_size / threads) + usedOverflowC, blurred.beginData + i * blurred.data_size / threads + usedOverflowC, bitmap.bmp_info_header.width + 4));
            }
        }
        for (int i = 0; i < threads; i++) {
            handlerC[i].join();
        }
        time = Timer::stop();
    }
    else {
        Timer::start();
        
        for (int i = 0; i < threads; i++) {
            uint8_t* begin = bitmap.beginData + i * baseSlice + usedOverflow;
            uint8_t* end = bitmap.beginData + (i + 1) * baseSlice + usedOverflow;
            uint8_t* bufferBegin = blurred.beginData + i * baseSlice + usedOverflow;
            if (overflow != 0) {
                handler.push_back(std::thread(gaussTrfAsm, begin, end + (int)processSize, bufferBegin, bitmap.bmp_info_header.width + 4));
                overflow--;
                usedOverflow+=processSize;
            }
            else {
                handler.push_back(std::thread(gaussTrfAsm, begin, end, bufferBegin, bitmap.bmp_info_header.width + 4));
            }
            Timer::check();
        }
        std::cout << "\n";
        for (int i = 0; i < handler.size(); i++) {
            handler[i].join();
            Timer::check();
        }
        
        /*
        for (int i = 0; i < threads; i++) {
            uint8_t* begin = bitmap.beginData + i * baseSlice + usedOverflow;
            uint8_t* end = bitmap.beginData + (i + 1) * baseSlice + usedOverflow;
            uint8_t* bufferBegin = blurred.beginData + i * baseSlice + usedOverflow;
            if (overflow != 0) {
                handler.push_back(std::thread(horizontalAsm, begin, end + processSize, bufferBegin, bitmap.bmp_info_header.width + 4));
                overflow--;
                usedOverflow += processSize;
            }
            else {
                handler.push_back(std::thread(horizontalAsm, begin, end, bufferBegin, bitmap.bmp_info_header.width + 4));
            }
        }
        for (int i = 0; i < handler.size(); i++) {
            handler[i].join();
        }
        int overflow = chunkNum % threads; //How many threads will have to take an extra chunk
        int usedOverflow = 0;
        handler.clear();
        
        for (int i = 0; i < threads; i++) {
            uint8_t* begin = blurred.beginData + i * baseSlice + usedOverflow;
            uint8_t* end = blurred.beginData + (i + 1) * baseSlice + usedOverflow;
            uint8_t* bufferBegin = bitmap.beginData + i * baseSlice + usedOverflow;
            if (overflow != 0) {
                handler.push_back(std::thread(verticalAsm, begin, end + processSize, bufferBegin, bitmap.bmp_info_header.width + 4));
                overflow--;
                usedOverflow += processSize;
            }
            else {
                handler.push_back(std::thread(verticalAsm, begin, end, bufferBegin, bitmap.bmp_info_header.width + 4));
            }
        }
        for (int i = 0; i < handler.size(); i++) {
            handler[i].join();
        }
        */
        time = Timer::stop();
    }
    bitmap.write("kopia.bmp");
    
}
