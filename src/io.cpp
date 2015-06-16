#include "io.h"
using namespace std;

template <class T>
string IO<T>::readAndRemoveAnnotation(string InputFileName)
{
    ifstream infile;
    infile.open(InputFileName.c_str(), ios::in);
    if(!infile)
    {
        cout<<"Fail to open:"<<InputFileName<<endl;
        cout<<"This program will be terminated!"<<endl;
        exit(0);
    }

    char buf[1024];
    string inbufer = " ";

    int linenum = 0;
    if(infile.is_open())
    {
        while(infile.good() && !infile.eof())
        {
            memset(buf,0,1024);
            infile.getline(buf,1024);
            string line = buf;
            //deal with empty line
            if(line.empty())
            {
                cout<<"line "<<linenum<<" was left out!"<<endl;
                continue;
            }
            //deal with a line begin with '#'
            int notation = line.find_first_not_of(" ", 0);
            if(line.compare(notation, 1, "#")==0)
            {
                cout<<"line "<<linenum<<" was commented away!"<<endl;
                continue;
            }
            //deal with a code line with annotation at the end
            vector<string> tmp;
            strtool::split(line, tmp, "#");
            if(tmp.size()>1)
            {
                line = tmp[0];
            }

            line = strtool::trim(line);

            vector<string> lines;
            string t = "a";
            strtool::split(line, lines, " ");
            vector<string>::iterator jter;
            for(jter = lines.begin(); jter<lines.end(); ++jter)
            {
                inbufer += *jter;
                inbufer += " ";
            }
            //inbufer += line;
            linenum++;
        }
    }
    infile.close();
    transform(inbufer.begin(), inbufer.end(), inbufer.begin(), strtool::to_upper);
    return inbufer;
}

template <class T>
void IO<T>::parseTypeOne(string inbufer,
    map<string, string> &dict_infos,
    map<string, string> &dict_items)
{

    //cout<<*each<<endl;
    string line = strtool::replace(inbufer, "}", "");
    line = strtool::replace(line, "(", "");
    line = strtool::replace(line, ")", "");
    vector<string> tmp;
    strtool::split(line, tmp, "{");

    //part one: in the parenthesis
    vector<string> tmp1;
    strtool::split(tmp[0], tmp1, "=");
    vector<string> record_infos;
    for(auto item=tmp1.begin(); item!=tmp1.end(); ++item)
    {
        vector<string> ttmp;
        strtool::split(*item, ttmp, ",");
        for(auto jtem=ttmp.begin(); jtem!=ttmp.end(); ++jtem)
        {
            string tttmp = strtool::trim(*jtem);
            if(!tttmp.empty())
            {
                record_infos.push_back(tttmp);
            }
        }
    }
    //map<string, string> dict_infos;
    int infon = record_infos.size();
    for(int i=0; i<infon/2; ++i)
    {
        pair<string, string> pair1 = \
            make_pair(record_infos[2*i], record_infos[2*i+1]);
        dict_infos.insert(pair1);
    }

    //part two: in the braket
    //each line represents one item
    //each item indicates a value of a key using the equality symbol.
    vector<string> tmp2;
    strtool::split(tmp[1], tmp2, "=");
    vector<string> record_items;
    for(auto item=tmp2.begin(); item!=tmp2.end(); ++item)
    {
        vector<string> ttmp;
        strtool::split(*item, ttmp, " ");
        for(auto jtem=ttmp.begin(); jtem!=ttmp.end(); ++jtem)
        {
            string tttmp = strtool::trim(*jtem);
            if(!tttmp.empty())
            {
                record_items.push_back(tttmp);
            }
        }
    }
    //map<string, string> dict_items;
    int itemn = record_items.size();
    for(int i=0; i<itemn/2; ++i)
    {
        pair<string, string> pair1 = \
            make_pair(record_items[2*i], record_items[2*i+1]);
        dict_items.insert(pair1);
    }
    /*
    for(auto item=dict_items.begin(); item!=dict_items.end(); ++item)
    {
        cout<<"##"<<item->second<<"##"<<endl;
    }*/
    /*
    for(auto item=dict_infos.begin(); item!=dict_infos.end(); ++item)
    {
        cout<<"##"<<item->first<<" "<<item->second<<"##"<<endl;
    }
    */
}

template <class T>
void IO<T>::create(string pathname)
{
    ofstream outfile;
    outfile.open(pathname.c_str(), ios::out);
    outfile.close();
}

template <class T>
void IO<T>::write(openphase::Storage3D<T, 0> &Field, string pathname, string info)
{
    int sizeX = Field.sizeX();
    int sizeY = Field.sizeY();
    int sizeZ = Field.sizeZ();
    stringstream outbufer;
    outbufer<<info<<endl;
    for(int x=0; x<sizeX; ++x)
    for(int y=0; y<sizeY; ++y)
    for(int z=0; z<sizeZ; ++z)
    {
        outbufer<<Field(x, y, z)<<endl;
    }
    outbufer<<endl;
    ofstream outfile;
    outfile.open(pathname.c_str(), ios::app);
    outfile<<outbufer.rdbuf();
    outfile.close();
}

template <class T>
void IO<T>::writeVTK(string outputfilename, openphase::Storage3D<double, 1> &var)
{
    stringstream outbufer;

    int Nx = var.sizeX();
    int Ny = var.sizeY();
    int Nz = var.sizeZ();
    int Nn = var.sizeN();
    
    outbufer << "# vtk DataFile Version 3.0\n";
    outbufer << "PhaseField\n";
    outbufer << "ASCII\n";
    outbufer << "DATASET STRUCTURED_GRID\n";
    outbufer << "DIMENSIONS " << Nx << " " << Ny << " " << Nz << "\n";
    outbufer << "POINTS " <<  Nx*Ny*Nz << " int\n";
    
    for(int k = 0; k < Nz; ++k)
    for(int j = 0; j < Ny; ++j)
    for(int i = 0; i < Nx; ++i)
    {
        outbufer << i << " " << j << " " << k << "\n";
    }
    
    outbufer << "\n";
    outbufer << "POINT_DATA " << Nx*Ny*Nz << "\n";
    
    for (int n = 0; n < Nn; n++)
    {
        outbufer << "SCALARS Variable_" << n << " double 1" << "\n";
        outbufer << "LOOKUP_TABLE default" << "\n";
        for(int k = 0; k < Nz; ++k)
        for(int j = 0; j < Ny; ++j)
        for(int i = 0; i < Nx; ++i)
        {
            outbufer << var(i,j,k,n) << "\n";
        }
    }
    
    ofstream outfile;
    outfile.open(outputfilename.c_str(), ios::out);
    if(outfile.is_open())
    {
        cout<<"----Output is done!"<<endl;
    }
    outfile << outbufer.rdbuf();
    outfile.close();
}

template <class T>
void IO<T>::writeVTK(string outputfilename, openphase::Storage3D<double, 2> &var)
{
    stringstream outbufer;

    int Nx = var.sizeX();
    int Ny = var.sizeY();
    int Nz = var.sizeZ();
    int Nn1 = var.sizeN1();
    int Nn2 = var.sizeN2();

    outbufer << "# vtk DataFile Version 3.0\n";
    outbufer << "PhaseField\n";
    outbufer << "ASCII\n";
    outbufer << "DATASET STRUCTURED_GRID\n";
    outbufer << "DIMENSIONS " << Nx << " " << Ny << " " << Nz << "\n";
    outbufer << "POINTS " <<  Nx*Ny*Nz << " int\n";

    for(int k = 0; k < Nz; ++k)
    for(int j = 0; j < Ny; ++j)
    for(int i = 0; i < Nx; ++i)
    {
        outbufer << i << " " << j << " " << k << "\n";
    }

    outbufer << "\n";
    outbufer << "POINT_DATA " << Nx*Ny*Nz << "\n";
    
    for (int n = 0; n < Nn1; ++n)
    {
        for (int m = 0; m < Nn2; ++m)
        {
            outbufer << "SCALARS Variable_" << n <<"_"<< m << " double 1" << "\n";
            outbufer << "LOOKUP_TABLE default" << "\n";
            for(int k = 0; k < Nz; ++k)
            for(int j = 0; j < Ny; ++j)
            for(int i = 0; i < Nx; ++i)
            {
               outbufer << var(i,j,k,n, m) << "\n";
            }
        }
    }
    
    ofstream outfile;
    outfile.open(outputfilename.c_str(), ios::out);
    if(outfile.is_open())
    {
        cout<<"----Output is done!"<<endl;
    }
    outfile << outbufer.rdbuf();
    outfile.close();
}

