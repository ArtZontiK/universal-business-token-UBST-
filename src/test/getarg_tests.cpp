#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/test/unit_test.hpp>

#include "util.h"

BOOST_AUTO_TEST_SUITE(getarg_tests)

static void
ResetArgs(const std::string& strArg)
{
    std::vector<std::string> vecArg;
    boost::split(vecArg, strArg, boost::is_space(), boost::token_compress_on);

    // Insert dummy executable name:
    vecArg.insert(vecArg.begin(), "testbitcoin");

    // Convert to char*:
    std::vector<const char*> vecChar;
    BOOST_FOREACH(std::string& s, vecArg)
        vecChar.push_back(s.c_str());

    ParseParameters(vecChar.size(), &vecChar[0]);
}

BOOST_AUTO_TEST_CASE(boolarg)
{
    ResetArgs("-ubst");
    BOOST_CHECK(GetBoolArg("-ubst"));
    BOOST_CHECK(GetBoolArg("-ubst", false));
    BOOST_CHECK(GetBoolArg("-ubst", true));

    BOOST_CHECK(!GetBoolArg("-fo"));
    BOOST_CHECK(!GetBoolArg("-fo", false));
    BOOST_CHECK(GetBoolArg("-fo", true));

    BOOST_CHECK(!GetBoolArg("-ubsto"));
    BOOST_CHECK(!GetBoolArg("-ubsto", false));
    BOOST_CHECK(GetBoolArg("-ubsto", true));

    ResetArgs("-ubst=0");
    BOOST_CHECK(!GetBoolArg("-ubst"));
    BOOST_CHECK(!GetBoolArg("-ubst", false));
    BOOST_CHECK(!GetBoolArg("-ubst", true));

    ResetArgs("-ubst=1");
    BOOST_CHECK(GetBoolArg("-ubst"));
    BOOST_CHECK(GetBoolArg("-ubst", false));
    BOOST_CHECK(GetBoolArg("-ubst", true));

    // New 0.6 feature: auto-map -nosomething to !-something:
    ResetArgs("-noubst");
    BOOST_CHECK(!GetBoolArg("-ubst"));
    BOOST_CHECK(!GetBoolArg("-ubst", false));
    BOOST_CHECK(!GetBoolArg("-ubst", true));

    ResetArgs("-noubst=1");
    BOOST_CHECK(!GetBoolArg("-ubst"));
    BOOST_CHECK(!GetBoolArg("-ubst", false));
    BOOST_CHECK(!GetBoolArg("-ubst", true));

    ResetArgs("-ubst -noubst");  // -ubst should win
    BOOST_CHECK(GetBoolArg("-ubst"));
    BOOST_CHECK(GetBoolArg("-ubst", false));
    BOOST_CHECK(GetBoolArg("-ubst", true));

    ResetArgs("-ubst=1 -noubst=1");  // -ubst should win
    BOOST_CHECK(GetBoolArg("-ubst"));
    BOOST_CHECK(GetBoolArg("-ubst", false));
    BOOST_CHECK(GetBoolArg("-ubst", true));

    ResetArgs("-ubst=0 -noubst=0");  // -ubst should win
    BOOST_CHECK(!GetBoolArg("-ubst"));
    BOOST_CHECK(!GetBoolArg("-ubst", false));
    BOOST_CHECK(!GetBoolArg("-ubst", true));

    // New 0.6 feature: treat -- same as -:
    ResetArgs("--ubst=1");
    BOOST_CHECK(GetBoolArg("-ubst"));
    BOOST_CHECK(GetBoolArg("-ubst", false));
    BOOST_CHECK(GetBoolArg("-ubst", true));

    ResetArgs("--noubst=1");
    BOOST_CHECK(!GetBoolArg("-ubst"));
    BOOST_CHECK(!GetBoolArg("-ubst", false));
    BOOST_CHECK(!GetBoolArg("-ubst", true));

}

BOOST_AUTO_TEST_CASE(stringarg)
{
    ResetArgs("");
    BOOST_CHECK_EQUAL(GetArg("-ubst", ""), "");
    BOOST_CHECK_EQUAL(GetArg("-ubst", "eleven"), "eleven");

    ResetArgs("-ubst -bar");
    BOOST_CHECK_EQUAL(GetArg("-ubst", ""), "");
    BOOST_CHECK_EQUAL(GetArg("-ubst", "eleven"), "");

    ResetArgs("-ubst=");
    BOOST_CHECK_EQUAL(GetArg("-ubst", ""), "");
    BOOST_CHECK_EQUAL(GetArg("-ubst", "eleven"), "");

    ResetArgs("-ubst=11");
    BOOST_CHECK_EQUAL(GetArg("-ubst", ""), "11");
    BOOST_CHECK_EQUAL(GetArg("-ubst", "eleven"), "11");

    ResetArgs("-ubst=eleven");
    BOOST_CHECK_EQUAL(GetArg("-ubst", ""), "eleven");
    BOOST_CHECK_EQUAL(GetArg("-ubst", "eleven"), "eleven");

}

BOOST_AUTO_TEST_CASE(intarg)
{
    ResetArgs("");
    BOOST_CHECK_EQUAL(GetArg("-ubst", 11), 11);
    BOOST_CHECK_EQUAL(GetArg("-ubst", 0), 0);

    ResetArgs("-ubst -bar");
    BOOST_CHECK_EQUAL(GetArg("-ubst", 11), 0);
    BOOST_CHECK_EQUAL(GetArg("-bar", 11), 0);

    ResetArgs("-ubst=11 -bar=12");
    BOOST_CHECK_EQUAL(GetArg("-ubst", 0), 11);
    BOOST_CHECK_EQUAL(GetArg("-bar", 11), 12);

    ResetArgs("-ubst=NaN -bar=NotANumber");
    BOOST_CHECK_EQUAL(GetArg("-ubst", 1), 0);
    BOOST_CHECK_EQUAL(GetArg("-bar", 11), 0);
}

BOOST_AUTO_TEST_CASE(doubledash)
{
    ResetArgs("--ubst");
    BOOST_CHECK_EQUAL(GetBoolArg("-ubst"), true);

    ResetArgs("--ubst=verbose --bar=1");
    BOOST_CHECK_EQUAL(GetArg("-ubst", ""), "verbose");
    BOOST_CHECK_EQUAL(GetArg("-bar", 0), 1);
}

BOOST_AUTO_TEST_CASE(boolargno)
{
    ResetArgs("-noubst");
    BOOST_CHECK(!GetBoolArg("-ubst"));
    BOOST_CHECK(!GetBoolArg("-ubst", true));
    BOOST_CHECK(!GetBoolArg("-ubst", false));

    ResetArgs("-noubst=1");
    BOOST_CHECK(!GetBoolArg("-ubst"));
    BOOST_CHECK(!GetBoolArg("-ubst", true));
    BOOST_CHECK(!GetBoolArg("-ubst", false));

    ResetArgs("-noubst=0");
    BOOST_CHECK(GetBoolArg("-ubst"));
    BOOST_CHECK(GetBoolArg("-ubst", true));
    BOOST_CHECK(GetBoolArg("-ubst", false));

    ResetArgs("-ubst --noubst");
    BOOST_CHECK(GetBoolArg("-ubst"));

    ResetArgs("-noubst -ubst"); // ubst always wins:
    BOOST_CHECK(GetBoolArg("-ubst"));
}

BOOST_AUTO_TEST_SUITE_END()
