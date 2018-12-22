#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <SDL2/SDL.h>
//#include <GL/glut.h>

using namespace std;

class SynthBaseClass {
protected:
    unsigned int id_ = 0;
public:
    SynthBaseClass(){}
    virtual ~SynthBaseClass() = default;
    virtual void drawIcon() = 0;
    virtual void NameAndID() = 0;

};
class Osc : public SynthBaseClass {
public:
    Osc(unsigned int id) {
        id_ = id;
    }
    ~Osc() {}
    void drawIcon() override {
        cout << "┌─┴───┐" << endl;
        cout << "│ osc │" << endl;
        cout << "└─┬───┘" << endl;
    }
    void NameAndID() override {
        cout << "Osc" << "     " << id_ << endl;
    }
};
class Truck : public SynthBaseClass {
public:
    Truck(unsigned int id) {
        id_ = id;
    }
    ~Truck() {}
    void drawIcon() override{
        cout << setw(20) << right << "-----.---_" << endl;
        cout << setw(20) << right << "-o-o-'o-o-" << endl;
    }
    void NameAndID() override {
        cout << "Truck" << "   " << id_ << endl;
    }
};



int main(int argc, char const *argv[])
{

    vector<SynthBaseClass*> v;
    v.reserve(1);

    // while ((++argv)[0])
    // {
    //     if (argv[0][0] == '-' )
    //     {
    //         switch (argv[0][1])  {

    //             case 'c':
    //                 v.push_back(new Osc());
    //                 break;

    //             case 't':
    //                 v.push_back(new Truck());
    //                 break;

    //             default:
    //                 break;
    //         }
    //     }
    // }


    string s;

    while (1) {
        cout << "Enter the name of the 1st object: ";
        cin >> s;
        cout << "you want: " << s << endl;
        if(s == "osc") v.push_back(new Osc(v.size()+1));
        if(s == "truck") v.push_back(new Truck(v.size()+1));
        if(s == "kill last") {
            if (v.size() > 0) {
                //v.back->dealock;
                v.pop_back();
            } else {
                cout << "nothing to kill."; 
            }
        } 
        if(s == "map") {
            for (int i = 0; i < v.size(); i++) {
                v[i]->NameAndID();        
            }
        }
        if(s == "kill") {
            cout << "Enter the index of the object to delete." << endl;
            int i = 0 ;
            cin >> i;
            v.erase(v.begin() + i);
        }

        for (int i = 0; i < v.size(); i++) {
            v[i]->drawIcon();        
        }
    }

    return 0;
}




// void drawIcon() override {
//         cout << "┌───┴─┐" << endl;
//         cout << "│ osc │" << endl;
//         cout << "└─┬───┘" << endl;
// }
// void drawIcon() override {
//         cout << "┌─────┐" << endl;
//         cout << "│ lfo │" << endl;
//         cout << "└───┬─┘" << endl;
// }
// void drawIcon() override {
//         cout << "┌─┴────┴─┐" << endl;
//         cout << "│ filter │" << endl;
//         cout << "└─┬──────┘" << endl;
// }
