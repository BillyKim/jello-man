#include "ModelLoader.h"
#include "ConvexCooker.h"
#include "ConcaveCooker.h"
#include "BinCooker.h"
#include "SoftbodyCooker.h"

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>

#if _DEBUG
    #define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

bool CookBin(Model* m, const string& outputFilename)
{
    BinCooker cooker(m, outputFilename);
    return cooker.Cook();
}
bool CookConvex(Model* m, const string& outputFilename)
{
    ConvexCooker cooker(m, outputFilename);
    return cooker.Cook();
}
bool CookConcave(Model* m, const string& outputFilename)
{
    ConcaveCooker cooker(m, outputFilename);
    return cooker.Cook();
}
bool CookSoftbody(Model* m, const string& outputFilename)
{
    SoftbodyCooker cooker(m, outputFilename);
    return cooker.Cook();
}

void PrefromAction(const string& action, Model* m)
{
    string outputFilename;
    cout << "Output filename: ";
    cin >> outputFilename;
    if (action == "0")
    {
        if (CookBin(m, outputFilename))
        {
            cout << "BinCooking succeeded!\n";
        }
        else
        {
            cout << "BinCooking FAILED!\n";
        }
    }
    else if (action == "1")
    {
        if(CookConvex(m, outputFilename))
        {
            cout << "ConvexCooking succeeded!\n";
        }
        else
        {
            cout << "ConvexCooking FAILED! Is the model convex? and triscount < 256? \n";
        }
    }
    else if (action == "2")
    {
        if (CookConcave(m, outputFilename))
        {
            cout << "ConcaveCooking succeeded!\n";
        }
        else
        {
            cout << "ConcaveCooking FAILED!\n";
        }
    }
    else if (action == "3")
    {
        if (CookSoftbody(m, outputFilename))
        {
            cout << "Softbodycooking succeeded!\n";
        }
        else
        {
            cout << "Softbodycooking FAILED!\n";
        }
    }
}

int main()
{
    ModelLoader* ml = new ModelLoader();


    cout << "********HappyCooker********\n";
    cout << "\n";
    bool stop = false;
    while (stop == false)
    {
        cout << "File:    -1 to stop\n";
        string file;
        cin >> file;
        if (file == "-1")
            break;
        Model* model = ml->Load(file);
        cout << "\n";
        
        while(model != 0)
        {
            cout << "Action: -1 stop; 0 cook bin; 1 cook convex; 2 cook concave, 3 cook softbody\n";
            string action;
            cin >> action;
            if (action == "-1")
                break;
            PrefromAction(action, model);
        }

        delete model;
    }
    delete ml;

    _CrtDumpMemoryLeaks();

    return 0;
}