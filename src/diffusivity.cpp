#include "diffusivity.h"

using namespace std;

Diffusivity::Diffusivity()
{
    setSymbolStrings();
    initSymbols();
    Index.push_back("A");
    Index.push_back("B");
    Index.push_back("C");
}

Diffusivity::Diffusivity(const Diffusivity &diff)
{
    for(auto each=diff.symbol_strings.begin(); each!=diff.symbol_strings.end(); ++each)
    {
        symbol_strings.push_back(*each);
    }
    initSymbols();
    for(auto each=diff.SymTable.begin(); each!=diff.SymTable.end(); ++each)
    {
        SymTable[each->first] = each->second;
    }
    FuncTable.insert(diff.FuncTable.begin(), diff.FuncTable.end());
    Index = diff.Index;
}

Diffusivity& Diffusivity::operator=(const Diffusivity &rhs)
{
    if(this != &rhs)
    {
        symbol_strings.clear();
        SymTable.clear();
        FuncTable.clear();
        for(auto each=rhs.symbol_strings.begin(); each!=rhs.symbol_strings.end(); ++each)
        {
            symbol_strings.push_back(*each);
        }
        initSymbols();
        for(auto each=rhs.SymTable.begin(); each!=rhs.SymTable.end(); ++each)
        {
            SymTable[each->first] = each->second;
        }
        FuncTable.insert(rhs.FuncTable.begin(), rhs.FuncTable.end());
        Index = rhs.Index;
    }
    return *this;
}

void Diffusivity::setSymbolStrings()
{
    symbol_strings.clear();
    symbol_strings.push_back("A_A0_0");
	symbol_strings.push_back("A_A0_1");
	symbol_strings.push_back("A_A0_2");
	symbol_strings.push_back("A_AB0_0");
	symbol_strings.push_back("A_AB0_1");
	symbol_strings.push_back("A_AB0_2");
	symbol_strings.push_back("A_AC0_0");
	symbol_strings.push_back("A_AC0_1");
	symbol_strings.push_back("A_AC0_2");
	symbol_strings.push_back("A_B0_0");
	symbol_strings.push_back("A_B0_1");
	symbol_strings.push_back("A_B0_2");
	symbol_strings.push_back("A_C0_0");
	symbol_strings.push_back("A_C0_1");
	symbol_strings.push_back("A_C0_2");

	symbol_strings.push_back("B_A0_0");
	symbol_strings.push_back("B_A0_1");
	symbol_strings.push_back("B_A0_2");
	symbol_strings.push_back("B_AB0_0");
	symbol_strings.push_back("B_AB0_1");
	symbol_strings.push_back("B_AB0_2");
	symbol_strings.push_back("B_B0_0");
	symbol_strings.push_back("B_B0_1");
	symbol_strings.push_back("B_B0_2");
	symbol_strings.push_back("B_BC0_0");
	symbol_strings.push_back("B_BC0_1");
	symbol_strings.push_back("B_BC0_2");
	symbol_strings.push_back("B_C0_0");
	symbol_strings.push_back("B_C0_1");
	symbol_strings.push_back("B_C0_2");

	symbol_strings.push_back("C_A0_0");
	symbol_strings.push_back("C_A0_1");
	symbol_strings.push_back("C_A0_2");
	symbol_strings.push_back("C_AC0_0");
	symbol_strings.push_back("C_AC0_1");
	symbol_strings.push_back("C_AC0_2");
	symbol_strings.push_back("C_B0_0");
	symbol_strings.push_back("C_B0_1");
	symbol_strings.push_back("C_B0_2");
	symbol_strings.push_back("C_BC0_0");
	symbol_strings.push_back("C_BC0_1");
	symbol_strings.push_back("C_BC0_2");
	symbol_strings.push_back("C_C0_0");
	symbol_strings.push_back("C_C0_1");
	symbol_strings.push_back("C_C0_2");
	symbol_strings.push_back("R");
}

void Diffusivity::initSymbols()
{
    for(auto each=symbol_strings.begin(); each!=symbol_strings.end(); ++each)
    {
        SymTable.insert(std::make_pair(*each, 0.0));
    }
    SymTable["R"] = 8.314;
}

void Diffusivity::setSymTable(std::map<std::string, double> _SymTable)
{
    for(auto iter=_SymTable.begin(); iter!=_SymTable.end(); ++iter)
    {
        if(SymTable.count(iter->first))
        {
            SymTable[iter->first]= iter->second;
        }
    }
}

void Diffusivity::setFuncTable(std::map<std::string, tfpointer> _FuncTable)
{
    FuncTable.insert(_FuncTable.begin(), _FuncTable.end());
}

void Diffusivity::showSymTable()
{
    for(auto iter=SymTable.begin(); iter!=SymTable.end(); ++iter)
    {
        std::cout<<iter->first<<": "<<iter->second<<std::endl;
    }
}

double Diffusivity::M_ANSATZ_A(std::vector<double> x, double T)
{
	double A_A0_0 = SymTable["A_A0_0"];
	double A_A0_1 = SymTable["A_A0_1"];
	double A_A0_2 = SymTable["A_A0_2"];
	double A_AB0_0 = SymTable["A_AB0_0"];
	double A_AB0_1 = SymTable["A_AB0_1"];
	double A_AB0_2 = SymTable["A_AB0_2"];
	double A_AC0_0 = SymTable["A_AC0_0"];
	double A_AC0_1 = SymTable["A_AC0_1"];
	double A_AC0_2 = SymTable["A_AC0_2"];
	double A_B0_0 = SymTable["A_B0_0"];
	double A_B0_1 = SymTable["A_B0_1"];
	double A_B0_2 = SymTable["A_B0_2"];
	double A_C0_0 = SymTable["A_C0_0"];
	double A_C0_1 = SymTable["A_C0_1"];
	double A_C0_2 = SymTable["A_C0_2"];
	double R = SymTable["R"];
	
	double x_A = x[0];
	double x_B = x[1];
	double x_C = x[2];
	double value = 0;
	if(T >= 298.15 && T < 6000.0)
	{
		value = exp((x_A*x_B*(A_AB0_0 + A_AB0_1*T + A_AB0_2*T*log(T)) + x_A*x_C*(A_AC0_0 + A_AC0_1*T + A_AC0_2*T*log(T)) + x_A*(A_A0_0 + A_A0_1*T + A_A0_2*T*log(T)) + x_B*(A_B0_0 + A_B0_1*T + A_B0_2*T*log(T)) + x_C*(A_C0_0 + A_C0_1*T + A_C0_2*T*log(T)))/(R*T))/(R*T);
	}
	return value;
}
double Diffusivity::M_ANSATZ_B(std::vector<double> x, double T)
{
	double B_A0_0 = SymTable["B_A0_0"];
	double B_A0_1 = SymTable["B_A0_1"];
	double B_A0_2 = SymTable["B_A0_2"];
	double B_AB0_0 = SymTable["B_AB0_0"];
	double B_AB0_1 = SymTable["B_AB0_1"];
	double B_AB0_2 = SymTable["B_AB0_2"];
	double B_B0_0 = SymTable["B_B0_0"];
	double B_B0_1 = SymTable["B_B0_1"];
	double B_B0_2 = SymTable["B_B0_2"];
	double B_BC0_0 = SymTable["B_BC0_0"];
	double B_BC0_1 = SymTable["B_BC0_1"];
	double B_BC0_2 = SymTable["B_BC0_2"];
	double B_C0_0 = SymTable["B_C0_0"];
	double B_C0_1 = SymTable["B_C0_1"];
	double B_C0_2 = SymTable["B_C0_2"];
	double R = SymTable["R"];
	
	double x_A = x[0];
	double x_B = x[1];
	double x_C = x[2];
	double value = 0;
	if(T >= 298.15 && T < 6000.0)
	{
		value = exp((x_A*x_B*(B_AB0_0 + B_AB0_1*T + B_AB0_2*T*log(T)) + x_A*(B_A0_0 + B_A0_1*T + B_A0_2*T*log(T)) + x_B*x_C*(B_BC0_0 + B_BC0_1*T + B_BC0_2*T*log(T)) + x_B*(B_B0_0 + B_B0_1*T + B_B0_2*T*log(T)) + x_C*(B_C0_0 + B_C0_1*T + B_C0_2*T*log(T)))/(R*T))/(R*T);
	}
	return value;
}
double Diffusivity::M_ANSATZ_C(std::vector<double> x, double T)
{
	double C_A0_0 = SymTable["C_A0_0"];
	double C_A0_1 = SymTable["C_A0_1"];
	double C_A0_2 = SymTable["C_A0_2"];
	double C_AC0_0 = SymTable["C_AC0_0"];
	double C_AC0_1 = SymTable["C_AC0_1"];
	double C_AC0_2 = SymTable["C_AC0_2"];
	double C_B0_0 = SymTable["C_B0_0"];
	double C_B0_1 = SymTable["C_B0_1"];
	double C_B0_2 = SymTable["C_B0_2"];
	double C_BC0_0 = SymTable["C_BC0_0"];
	double C_BC0_1 = SymTable["C_BC0_1"];
	double C_BC0_2 = SymTable["C_BC0_2"];
	double C_C0_0 = SymTable["C_C0_0"];
	double C_C0_1 = SymTable["C_C0_1"];
	double C_C0_2 = SymTable["C_C0_2"];
	double R = SymTable["R"];
	
	double x_A = x[0];
	double x_B = x[1];
	double x_C = x[2];
	double value = 0;
	if(T >= 298.15 && T < 6000.0)
	{
		value = exp((x_A*x_C*(C_AC0_0 + C_AC0_1*T + C_AC0_2*T*log(T)) + x_A*(C_A0_0 + C_A0_1*T + C_A0_2*T*log(T)) + x_B*x_C*(C_BC0_0 + C_BC0_1*T + C_BC0_2*T*log(T)) + x_B*(C_B0_0 + C_B0_1*T + C_B0_2*T*log(T)) + x_C*(C_C0_1*T + C_C0_0 + C_C0_2*T*log(T)))/(R*T))/(R*T);
	}
	return value;
}

std::map<std::string, double> Diffusivity::DI_ANSATZ(std::vector<double> x, double T)
{
    std::map<std::string, double> TFValue;
    std::map<std::string, double> MValue;
    
    TFValue.insert(std::make_pair("AA", FuncTable["AA"](x, T)));
    TFValue.insert(std::make_pair("AB", FuncTable["AB"](x, T)));
    TFValue.insert(std::make_pair("AC", FuncTable["AC"](x, T)));

    TFValue.insert(std::make_pair("BA", FuncTable["BA"](x, T)));
    TFValue.insert(std::make_pair("BB", FuncTable["BB"](x, T)));
    TFValue.insert(std::make_pair("BC", FuncTable["BC"](x, T)));

    TFValue.insert(std::make_pair("CA", FuncTable["CA"](x, T)));
    TFValue.insert(std::make_pair("CB", FuncTable["CB"](x, T)));
    TFValue.insert(std::make_pair("CC", FuncTable["CC"](x, T)));

    MValue.insert(std::make_pair("A", M_ANSATZ_A(x, T)));
    MValue.insert(std::make_pair("B", M_ANSATZ_B(x, T)));
    MValue.insert(std::make_pair("C", M_ANSATZ_C(x, T)));

    std::map<std::string, double> c;
    c.insert(std::make_pair("A", x[0]));
    c.insert(std::make_pair("B", x[1]));
    c.insert(std::make_pair("C", x[2]));


    std::map<std::string, double> DIValue;
    for(auto item=Index.begin(); item!=Index.end(); ++item)
    {
        for(auto jtem=Index.begin(); jtem!=Index.end(); ++jtem)
        {
            double divalue = 0.0;
            for(auto ktem=Index.begin(); ktem!=Index.end(); ++ktem)
            {
                double delta = 0.0;
                if(*item == *ktem) delta = 1.0;
                divalue += (delta - c[*item])*c[*ktem]*MValue[*ktem]*TFValue[*ktem+*jtem];
            }
            DIValue.insert(std::make_pair(*item+*jtem, divalue));
        }
    }

    return DIValue;
}

void Diffusivity::calculateDiffusivity(
        openphase::Storage3D<double, 1> & conc,
        openphase::Storage3D<double, 2> & D,
        double Temperature)
{
    for(int nx=0; nx<conc.sizeX(); ++nx)
    for(int ny=0; ny<conc.sizeY(); ++ny)
    for(int nz=0; nz<conc.sizeN(); ++nz)
    {
        openphase::Tensor<double, 1> c = conc(nx, ny, nz);
        double eps = 1.0e-10;
        std::vector<double> x;
        x.resize(3);
        x[0] = c(0)<eps?eps:(c(0)>1.0-eps?1.0-eps:c(0));
        x[1] = c(1)<eps?eps:(c(1)>1.0-eps?1.0-eps:c(1));
        x[2] = c(2)<eps?eps:(c(2)>1.0-eps?1.0-eps:c(2));
        std::map<std::string, double> DIValue = DI_ANSATZ(x, Temperature);
        for(int nci=0; nci<D.sizeN1(); ++nci)
        for(int ncj=0; ncj<D.sizeN2(); ++ncj)
        {
            D(nx, ny, nz, nci, ncj) = DIValue[Index[nci]+Index[ncj]] \
                                    - DIValue[Index[nci]+Index[2]];
        }
    }
}
