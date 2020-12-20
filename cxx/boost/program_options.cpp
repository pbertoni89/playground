#include <boost/program_options.hpp>
#include <iostream>
#include <algorithm>
#include <iterator>

using namespace boost;
using namespace boost::program_options;


using namespace std;
// A helper function to simplify the main part.
template<class T>
ostream& operator<<(ostream& os, const vector<T>& v)
{
    copy(v.begin(), v.end(), ostream_iterator<T>(cout, " "));
    return os;
}


void on_age(int age)
{
  std::cout << "On age: " << age << '\n';
}


void demo_1(int argc, const char *argv[])
{
    try
    {
        options_description desc{"Options"};
        desc.add_options()
            ("help,h", "Help screen")
            ("pi", value<float>()->default_value(3.14f), "Pi")
            ("age", value<int>()->notifier(on_age), "Age");

        variables_map vm;
        store(parse_command_line(argc, argv, desc), vm);
        notify(vm);

        if (vm.count("help"))
            std::cout << desc << '\n';
        else if (vm.count("age"))
            std::cout << "Age: " << vm["age"].as<int>() << '\n';
        else if (vm.count("pi"))
            std::cout << "Pi: " << vm["pi"].as<float>() << '\n';
    }
    catch (const error &ex)
    {
        std::cerr << ex.what() << '\n';
    }
}


int demo_2(int argc, const char ** argv)
{
    bool bOnline;
    int opt;
    int portnum;
    try {
        options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("optimization",
                value<int>(&opt)->default_value(10),
                "optimization level")
            ("verbose,v",
                value<int>()->implicit_value(1),
                "enable verbosity (optionally specify level)")
            ("listen,l",
                value<int>(&portnum)->implicit_value(1001)->default_value(0,"no"),
                "listen on a port.")
            ("online,o",
                value<bool>(& bOnline)->implicit_value(true)->default_value(false),
                "framework v2 online")
            ("include-path,I",
                value< vector<string> >(),
                "include path");

        variables_map vm;
        // store(command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
        store(parse_command_line(argc, argv, desc), vm);
        notify(vm);

        if (vm.count("help")) {
            cout << "Usage: options_description [options]\n";
            cout << desc;
            return 0;
        }
        if (vm.count("include-path"))
        {
            cout << "Include paths are: "
                 << vm["include-path"].as< vector<string> >() << "\n";
        }
        if (vm.count("online"))
        {
            cout << "online enabled\n";
        }
        if (vm.count("input-file"))
        {
            cout << "Input files are: "
                 << vm["input-file"].as< vector<string> >() << "\n";
        }
        if (vm.count("verbose")) {
            cout << "Verbosity enabled.  Level is " << vm["verbose"].as<int>()
                 << "\n";
        }
        cout << "Optimization level is " << opt << "\n";
        cout << "Listen port is " << portnum << "\n";
        cout << "Online is " << bOnline << "\n";
    }
    catch(std::exception& e)
    {
        cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}


int main (int argc, const char ** argv)
{
    return demo_2(argc, argv);
}
