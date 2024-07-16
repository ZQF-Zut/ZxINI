#include <print>
#include <cassert>
#include <iostream>
#include <ZxINI/Value.h>
#include <ZxINI/Parser.h>


[[maybe_unused]] static auto TestValue(void) -> void
{
    ZQF::ZxINI::Value v0 = 123;
    assert(v0.Get<size_t>() == 123);

    ZQF::ZxINI::Value v1 = 13.131;
    assert(v1.Get<double>() == 13.131);

    ZQF::ZxINI::Value v2 = "2389y9b vuihv 789hvuiuih 89wef90120- fdkcpqwe p[c,[";
    assert(v2.Get<std::string_view>() == "2389y9b vuihv 789hvuiuih 89wef90120- fdkcpqwe p[c,[");

    ZQF::ZxINI::Value v3 = std::string("w fci9h249if jn892jg89jwe89fmc 89w3e g890f3 m90");
    assert(v3.Get<std::string_view>() == "w fci9h249if jn892jg89jwe89fmc 89w3e g890f3 m90");

    std::string m4 = "wegwgwegsdvmikwjegopj490j9034g890je0wvn023mv";
    ZQF::ZxINI::Value v4 = m4;
    assert(m4.size());
    assert(v4.Get<std::string_view>() == "wegwgwegsdvmikwjegopj490j9034g890je0wvn023mv");

    const std::string m5 = "r4rbeb3egwegsdsbwgsdbwe4b34b34b34";
    ZQF::ZxINI::Value v5 = m5;
    assert(m5.size());
    assert(v5.Get<std::string_view>() == "r4rbeb3egwegsdsbwgsdbwe4b34b34b34");

    std::string m6 = "11124124124124214121241241241241";
    const char* m6_ptr = m6.data();
    ZQF::ZxINI::Value v6 = std::move(m6);
    assert(v6.Get<std::string_view>() == "11124124124124214121241241241241");
    assert(v6.Get<const char*>() == m6_ptr);


    ZQF::ZxINI::Value v7 = v0;
    assert(v7.Get<size_t>() == 123);
    assert(v0.Get<size_t>() == 123);

    const char* v2_v_ptr = v2.Get<std::string_view>().data();
    ZQF::ZxINI::Value v8 = std::move(v2);
    assert(v8.Get<std::string_view>() == "2389y9b vuihv 789hvuiuih 89wef90120- fdkcpqwe p[c,[");
    assert(v8.Get<const char*>() == v2_v_ptr);

    [[maybe_unused]] int x = 0;
}

[[maybe_unused]] static auto TestParser(void) -> void
{
    ZQF::ZxINI::Parser parser;
    parser.Parse("1.ini");
    parser.Save("2.ini", true);
}


auto main(void) -> int
{
    try
    {
        TestValue();
        TestParser();
    }
    catch (const std::exception& err)
    {
        std::println(std::cerr, "std::exception: {}", err.what());
    }
}
