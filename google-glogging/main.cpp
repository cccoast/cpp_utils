#include "gLogHelper.h"
#include <gflags/gflags.h>
#include <gtest/gtest.h>
#include <string>

using namespace std;

DEFINE_string(name,"","type in your name");
DEFINE_int32(age,-1,"type in your age");

static bool ValidateName(const char* flagname, const std::string &value) {
	if (value.length() > 10)
	{
		return true;
	}
	return false;
}

static bool ValidateAge(const char* flagname, google::int32 value) {
	if (value > 0)
	{
		return true;
	}
	return false;
}

auto valid_name = google::RegisterFlagValidator(&FLAGS_name, &ValidateName);
auto valid_age = google::RegisterFlagValidator(&FLAGS_age, &ValidateAge);

TEST(GOOGLE,LOG){
    GLogHelper gh("google-test");
    LOG(INFO)<<"my name is xudi";
    LOG(ERROR)<<"hello world";
}

void gflags_test(int argc,char* argv[]){

    google::ParseCommandLineFlags(&argc, &argv, false);

    string _name = FLAGS_name;
    int _age = FLAGS_age;

    cout << _name << endl;
    cout << _age  << endl;

    google::ShutDownCommandLineFlags();
}

int main(int argc,char* argv[])
{
    //int argc = 2;
    //char* argv[] = { "google_test","--gtest_filter=GOOGLE.FLAG"};
    //::testing::InitGoogleTest(&argc,argv);
    gflags_test(argc,argv);
    return 0;
}
