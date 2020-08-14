#include<iostream>
#include<conio.h>
#include<string.h>
#include<fstream>
#include <string>
#include <conio.h>
#include <filesystem>
#define getch _getch
#define getche _getche

using namespace std;
class Employee
{
public:
    enum EEmployeeType
    {
        eSalaried,
        eHourly,
        eCommision
    };

    virtual void set_data() = 0;
    virtual int get_id() const final { return id; }
    virtual string get_name() const final { return name; }
    virtual double get_salary() const = 0;
    virtual double compute_salary() { return get_salary(); }
    virtual EEmployeeType get_employee_type() const = 0;
    virtual bool write_data( const std::string & fileName ) const
    {
        std::ofstream ofs( fileName, std::ofstream::binary | std::ofstream::app );
        return write_data( ofs );
    }
    virtual bool write_data( ofstream & ofs ) const 
    {
        if ( !ofs.good() )
            return false;

        auto type = get_employee_type();
        ofs.write( reinterpret_cast<const char *>(&type), sizeof( EEmployeeType ) );

        ofs.write( reinterpret_cast<const char *>(&id), sizeof( int ) );
        auto sz = name.size();
        ofs.write( reinterpret_cast<const char *>(&sz), sizeof( size_t ) );
        ofs.write( name.c_str(), sz );

        return ofs.good();
    }

    virtual bool read_data( ifstream & ifs ) final
    {
        if ( !ifs.good() )
            return false;

        ifs.read( reinterpret_cast<char *>(&id), sizeof( int ) );
        size_t sz;
        ifs.read( reinterpret_cast<char *>(&sz), sizeof( size_t ) );

        char * tmp = new char[ sz + 1 ];
        ifs.read( tmp, sz );
        tmp[ sz ] = 0;
        name = tmp;
        return read_my_data( ifs );
    }

    virtual bool read_my_data( ifstream & ifs )=0;

    static Employee * constructFromData( const std::string & fileName );
    static Employee * constructFromData( std::ifstream & ifs );

    virtual bool modify() 
    {
        display();
        cout << "\n";

        bool retVal = false;
        int newID = 0;
        cout << " Enter the ID of the Employee (Currently: " << id << ")\n";
        cin >> newID;
        retVal = retVal || newID != id;
        id = newID;

        cin.ignore();
        cout << " Enter the name of the Employee (Currently: " << name << ")\n";

        std::string tmp;
        getline( cin, tmp );
        retVal = retVal || tmp!= name;
        id = newID;

        return modify_my_data() || retVal;
    }

    virtual bool modify_my_data() = 0;

    virtual void display() const
    {
        cout << " ID of the Employee: " << get_id() << endl;
        cout << " Name of the Employee: " << get_name() << endl;
        cout << " Salary of Employee: " << get_salary() << endl;
    }
protected:
    int id;
    string name;
};

class SalariedEmp : public Employee
{
private:
    double salary{ 0.0 };
public:
    void set_data()
    {
        cout << " Enter the ID of the Employee " << endl;
        cin >> id;
        cin.ignore();
        cout << " Enter the name of the Employee " << endl;
        getline( cin, name );
        cout << " Enter the Salary of the Employee " << endl;
        cin >> salary;
        cout << " Data Entered Successfully " << endl;
    }
    virtual EEmployeeType get_employee_type() const override { return EEmployeeType::eSalaried; }
    double get_salary() const override
    {
        return salary;
    }
    bool write_data( ofstream & ofs ) const override
    {
        bool retVal = Employee::write_data( ofs );
        if ( !retVal )
            return false;

        ofs.write( reinterpret_cast<const char *>(&salary), sizeof( double ) );

        return ofs.good();
    }

    virtual bool read_my_data( ifstream & ifs ) override
    {
        if ( !ifs.good() )
            return false;

        ifs.read( reinterpret_cast<char *>(&salary), sizeof( double ) );

        return ifs.good();
    }

    virtual bool modify_my_data()
    {
        double newSalary = 0.0;
        cout << " Enter the Salary of the Employee (Currently: " << salary << ")\n";
        cin >> newSalary;
        bool retVal = newSalary != salary;
        salary = newSalary;
        cin.ignore();
        return retVal;
    }

};

class HourlyEmp : public Employee
{
private:
    double rate{ 0.0 };
    int hours{ 0 };
public:
    void set_data()
    {
        cout << " Enter the ID of the Employee " << endl;
        cin >> id;
        cin.ignore();
        cout << " Enter the name of the Employee " << endl;
        getline( cin, name );
        cout << " Enter the Rate per hour of the Employee " << endl;
        cin >> rate;
        cout << " Data Entered Successfully " << endl;
    }
    virtual EEmployeeType get_employee_type() const override { return EEmployeeType::eHourly; }
    double get_rate() const
    {
        return rate;
    }

    double get_salary() const override
    {
        double salary;
        salary = hours * rate;
        return salary;
    }

    double compute_salary() override
    {
        cout << " Enter the Number of hours He Worked " << endl;
        cin >> hours;
        return get_salary();
    }

    bool write_data( ofstream & ofs ) const override
    {
        bool retVal = Employee::write_data( ofs );
        if ( !retVal )
            return false;

        ofs.write( reinterpret_cast<const char *>(&rate), sizeof( double ) );
        ofs.write( reinterpret_cast<const char *>(&hours), sizeof( int ) );

        return ofs.good();
    }
    virtual bool read_my_data( ifstream & ifs ) override
    {
        if ( !ifs.good() )
            return false;

        ifs.read( reinterpret_cast<char *>(&rate), sizeof( double ) );
        ifs.read( reinterpret_cast<char *>(&hours), sizeof( int ) );

        return ifs.good();
    }

    virtual bool modify_my_data()
    {
        double newRate = 0.0;
        cout << " Enter the Rate of the Employee (Currently: " << rate << ")\n";
        cin >> newRate;
        bool retVal = newRate != rate;
        rate = newRate;
        cin.ignore();

        int newHours = 0;
        cout << " Enter the Hours of the Employee (Currently: " << hours << ")\n";
        cin >> newHours;
        retVal = retVal || ( newHours != hours );
        hours = newHours;
        cin.ignore();

        return retVal;
    }

};


class CommEmp : public Employee
{
private:
    double comm_rate{ 0.0 };
    double sales{0.0};
public:
    void set_data()
    {
        cout << " Enter the ID of the Employee " << endl;
        cin >> id;
        cin.ignore();
        cout << " Enter the name of the Employee " << endl;
        getline( cin, name );
        cout << " Enter the Commission Rate of the Employee " << endl;
        cin >> comm_rate;
        cout << " Data Entered Successfully " << endl;
    }
    double get_salary() const override
    {
        double salary;
        salary = sales * comm_rate;
        return salary;
    }
    
    double compute_salary() override
    {
        cout << " Enter His Sales To calculate his Salary " << endl;
        cin >> sales;
        return get_salary();
    }

    virtual EEmployeeType get_employee_type() const override { return EEmployeeType::eCommision; }

    bool write_data( ofstream & ofs ) const override
    {
        bool retVal = Employee::write_data( ofs );
        if ( !retVal )
            return false;

        ofs.write( reinterpret_cast<const char *>(&comm_rate), sizeof( double ) );
        ofs.write( reinterpret_cast<const char *>(&sales), sizeof( double ) );

        return ofs.good();
    }

    virtual bool read_my_data( ifstream & ifs ) override
    {
        if ( !ifs.good() )
            return false;

        ifs.read( reinterpret_cast<char *>(&comm_rate), sizeof( double ) );
        ifs.read( reinterpret_cast<char *>(&sales), sizeof( double ) );

        return ifs.good();
    }

    virtual bool modify_my_data()
    {
        double newRate = 0.0;
        cout << " Enter the Rate of the Employee (Currently: " << comm_rate << ")\n";
        cin >> newRate;
        bool retVal = newRate != comm_rate;
        comm_rate = newRate;
        cin.ignore();

        double newSales = 0.0;
        cout << " Enter the Sales of the Employee (Currently: " << sales << ")\n";
        cin >> newSales;
        retVal = retVal || ( newSales != sales );
        sales = newSales;
        cin.ignore();

        return retVal;
    }

};


Employee * Employee::constructFromData( const std::string & fileName )
{
    ifstream ifs( fileName, std::ifstream::in | std::ifstream::binary );
    if ( !ifs.good() )
    {
        std::cout << "Could not open file: '" << fileName << "'\n";
        return nullptr;
    }

    return constructFromData( ifs );
}

Employee * Employee::constructFromData( std::ifstream & ifs )
{
    if ( !ifs.good() || ifs.eof() )
        return nullptr;

    EEmployeeType type;
    ifs.read( reinterpret_cast<char *>(&type), sizeof( EEmployeeType ) );
    if ( !ifs.good() || ifs.eof() )
        return nullptr;

    Employee * retVal = nullptr;
    switch ( type )
    {
        case EEmployeeType::eSalaried:
            {
                retVal = new SalariedEmp();
                if ( !retVal->read_data( ifs ) )
                {
                    delete retVal;
                    return nullptr;
                }
                break;
            }
        case EEmployeeType::eCommision:
            {
                retVal = new CommEmp();
                if ( !retVal->read_data( ifs ) )
                {
                    delete retVal;
                    return nullptr;
                }
                break;
            }

        case EEmployeeType::eHourly:
            {
                retVal = new HourlyEmp();
                if ( !retVal->read_data( ifs ) )
                {
                    delete retVal;
                    return nullptr;
                }
                break;
            }
        default:
            {
                std::cout << "Invalid employee type read in.\n";
                return nullptr;
            }
    }
    return retVal;
}



void SalariedEmployee()  // Add
{
    Employee * obj;
    ofstream ofs( "Employee.bin", ios_base::app | std::ofstream::binary );
    obj = new SalariedEmp;
    obj->set_data();
    obj->write_data( ofs );
    ofs.close();
    getch();
}

void HourlyEmployee()   // Add
{
    Employee * obj;
    ofstream ofs( "Employee.bin", ios_base::app | std::ofstream::binary  );
    obj = new HourlyEmp;
    obj->set_data();
    obj->write_data( ofs );
    ofs.close();
    getch();
}

void CommissionedEmployee()  // Add
{
    Employee * obj;
    ofstream ofs( "Employee.bin", ios_base::app | std::ofstream::binary );
    obj = new CommEmp;
    obj->set_data();
    obj->write_data( ofs );
    ofs.close();
    getch();
}

Employee * findEmployee( int employeeID, std::ifstream & ifs )
{
    Employee * obj;
    while ( (obj = Employee::constructFromData( ifs )) != nullptr )
    {
        if ( obj->get_id() == employeeID )
        {
            return obj;
        }
    }
    return nullptr;
}

void DispEmployee()
{
    int x;
    cout << "Enter the ID of the Employee to Display " << endl;
    cin >> x;
    ifstream ifs( "Employee.bin", std::ifstream::in | std::ifstream::binary );

    Employee * employee = findEmployee( x, ifs );
    if ( employee )
        employee->display();
    else
        cout << " No Data Found " << endl;

    getch();
}

void DeleteEmployee( int employeeID )
{
    ifstream ifs( "Employee.bin", std::ifstream::in | std::ifstream::binary );
    ofstream ofs( "temp.bin", std::ofstream::out | std::ofstream::binary );

    bool found = false;
    Employee * obj;
    while ( (obj = Employee::constructFromData( ifs )) != nullptr )
    {
        if ( obj->get_id() != employeeID )
        {
            obj->write_data( ofs );
            found = true;
        }
    }
    if ( found )
        cout << "Employee successfully deleted\n";
    else
        cout << "Could not find Employee\n";

    ofs.close();
    ifs.close();
    try
    {
        std::filesystem::rename( "Employee.bin", "Employee.bin.bak" );
        std::filesystem::rename( "temp.bin", "Employee.bin" );
    }
    catch( std::filesystem::filesystem_error & e )
    {
        std::cout << e.what() << "\n";
    }
}

void DeleteEmployee()
{
    int x;
    cout << " Enter the ID of the Employee to Modify " << endl;
    cin >> x;
    DeleteEmployee( x );
}

void ModifyEmployee()
{
    int x;  
    cout << " Enter the ID of the Employee to Modify" << endl;
    cin >> x;

    ifstream ifs( "Employee.bin", std::ifstream::in | std::ifstream::binary );

    Employee * obj = findEmployee( x, ifs );
    if ( !obj )
    {
        cout << " Could not load employee" << endl;
        return;
    }
    ifs.close();

    if ( !obj->modify() )
        return;

    DeleteEmployee( x );

    ofstream ofs( "Employee.bin", std::ofstream::app | std::ofstream::binary );

    obj->write_data( ofs );
    cout << "Employee Modified successfully" << "\n";
    getch();
}

void AddEmployee()
{
    char ch;
    while ( 1 )
    {
        cout << " 1.Salaried Employee " << endl;
        cout << " 2.Hourly Employee " << endl;
        cout << " 3.Comissioned Employee " << endl;
        cout << " 4.Main Menu " << endl;
        ch = getche();
        if ( ch == '1' )
            SalariedEmployee();
        else if ( ch == '2' )
            HourlyEmployee();
        else if ( ch == '3' )
            CommissionedEmployee();
        else if ( ch == '4' )
            break;
        else
            cout << " Invalid Input " << endl;
    }
    getch();
}
int main()
{
    char ch;
    while ( 1 )
    {
        system( "cls" );
        cout << " 1.Add Employee " << endl;
        cout << " 2.Display Employee " << endl;
        cout << " 3.Modify Employee " << endl;
        cout << " 4.Delete Employee " << endl;
        cout << " 5.Calculate Salary of Employee " << endl;
        cout << " 6.Exit " << endl;
        ch = getche();
        if ( ch == '1' )
            AddEmployee();
        else if ( ch == '2' )
            DispEmployee();
        else if ( ch == '3' )
            ModifyEmployee();
        else if ( ch == '4' )
            DeleteEmployee();
        else if ( ch == '6' )
            break;
        else
            cout << " Invalid Input " << endl;
    }
    getch();
    return 0;
}
