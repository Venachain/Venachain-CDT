#include "log.h"
//#include "bcwasm/name.hpp"
#include "bcwasm/print.hpp"
#include "bcwasm/storagetype.hpp"
#include "unittest.hpp"

using namespace bcwasm;

TEST_CASE(testStorage, uint8) {
  typedef bcwasm::Uint8<"test_uint8"_n> TestUint8;

  TestUint8 t;
  *t = 10;

  ASSERT(*t == 10);
  ASSERT(t.get() == 10);
}

TEST_CASE(testStorage, int8) {
  typedef bcwasm::Int8<"test_int8"_n> TestInt8;
  // set
  {
    TestInt8 t;
    *t = 10;

    ASSERT(*t == 10);
    ASSERT(t.get() == 10);
  }

  // get
  {
    TestInt8 t;
    ASSERT(*t == 10);
    ASSERT(t.get() == 10);
  }
}

TEST_CASE(testStorage, uint16) {
  typedef bcwasm::Uint16<"test_uint16"_n> TestUint16;
  // set
  {
    TestUint16 t;
    *t = 10;

    ASSERT(*t == 10);
    ASSERT(t.get() == 10);
  }

  // get
  {
    TestUint16 t;
    ASSERT(*t == 10);
    ASSERT(t.get() == 10);
  }
}

TEST_CASE(testStorage, int16) {
  typedef bcwasm::Int16<"test_int16"_n> TestInt16;
  // set
  {
    TestInt16 t;
    *t = 666;

    ASSERT(*t == 666);
    ASSERT(t.get() == 666);
  }

  // get
  {
    TestInt16 t;
    ASSERT(*t == 666);
    ASSERT(t.get() == 666);
  }
}

TEST_CASE(testStorage, uint) {
  typedef bcwasm::Uint<"test_uint"_n> TestUint;
  // set
  {
    TestUint t;
    *t = 6767;

    ASSERT(*t == 6767);
    ASSERT(t.get() == 6767);
  }

  // get
  {
    TestUint t;
    ASSERT(*t == 6767);
    ASSERT(t.get() == 6767);
  }
}

TEST_CASE(testStorage, int) {
  typedef bcwasm::Int<"test_int"_n> TestInt;
  // set
  {
    TestInt t;
    *t = 678;

    ASSERT(*t == 678);
    ASSERT(t.get() == 678);
  }

  // get
  {
    TestInt t;
    ASSERT(*t == 678);
    ASSERT(t.get() == 678);
  }
}

TEST_CASE(testStorage, uint64) {
  typedef bcwasm::Uint64<"test_uint64"_n> TestUint64;
  // set
  {
    TestUint64 t;
    *t = 888;

    ASSERT(*t == 888);
    ASSERT(t.get() == 888);
  }

  // get
  {
    TestUint64 t;
    ASSERT(*t == 888);
    ASSERT(t.get() == 888);
  }
}

TEST_CASE(testStorage, int64) {
  typedef bcwasm::Int64<"test_int64"_n> TestInt64;
  // set
  {
    TestInt64 t;
    *t = 8888;

    ASSERT(*t == 8888);
    ASSERT(t.get() == 8888);
  }

  // get
  {
    TestInt64 t;
    ASSERT(*t == 8888);
    ASSERT(t.get() == 8888);
  }
}

TEST_CASE(testStorage, string) {
  typedef bcwasm::String<"test_string"_n> TestString;
  {
    TestString str;
    *str = "hello";
    ::clearLog();
    bcwasm::print(str->c_str());
    ASSERT(test::getLog() == "hello");

    ::clearLog();
    bcwasm::print((*str).c_str());
    ASSERT(test::getLog() == "hello");

    ::clearLog();
    str->append("bcwasm");
    bcwasm::print(str->c_str());
    ASSERT(test::getLog() == "hellovenachain");

    ::clearLog();
    str->assign("Hello, venachain!");
    bcwasm::print(str->c_str());
    ASSERT(test::getLog() == "Hello, venachain!");

    ::clearLog();
    bcwasm::print(str.get().c_str());
    ASSERT(test::getLog() == "Hello, venachain!");
  }

  // get
  {
    TestString str;
    ::clearLog();
    bcwasm::print((*str));
    ASSERT(test::getLog() == "Hello, venachain!");
  }
}

TEST_CASE(testStorage, vector) {
  typedef bcwasm::Vector<"test_vector"_n, int> TestVec;
  {
    TestVec vec;
    for (auto i = 0; i < 10; i++) {
      (*vec).push_back(i);
    }

    for (auto i = 0; i < 10; i++) {
      ASSERT((*vec)[i] == i);
    }
    ASSERT(vec.get()[0] == 0);
    ASSERT(vec->front() == 0);
    ASSERT(vec->back() == 9);

    int i = 0;
    for (auto it = vec->begin(); it != vec->end(); ++it, i++) {
      ASSERT(*it == i);
    }

    vec->clear();
    ASSERT(vec->size() == 0);

    for (auto i = 0; i < 10; i++) {
      vec->push_back(i);
    }
    ASSERT(vec->size() == 10);
  }

  // get
  {
    TestVec vec;
    ASSERT(vec->size() == 10);
    ASSERT(vec->front() == 0);
    ASSERT(vec->back() == 9);
    ASSERT((*vec)[5] == 5);
  }
}

TEST_CASE(testStorage, set) {
  typedef bcwasm::Set<"test_set"_n, int> TestSet;
  {
    TestSet ts;
    for (auto i = 0; i < 10; i++) {
      (*ts).insert(i);
    }

    int i = 0;
    for (auto it = ts->begin(); it != ts->end(); ++it, i++) {
      ASSERT(*it == i);
    }

    ts->erase(8);
    ASSERT(ts->size() == 9);

    ts->clear();
    ASSERT(ts->size() == 0);

    ts->insert(1);
    ts->insert(2);
    ts->insert(3);
  }

  // get
  {
    TestSet ts;
    ASSERT(ts->size() == 3);
    int i = 1;
    for (auto it = ts->begin(); it != ts->end(); ++it, i++) {
      ASSERT(*it == i);
    }
  }
}

TEST_CASE(testStorage, map) {
  typedef bcwasm::Map<"test_map"_n, std::string, std::string> TestMap;
  // set
  {
    TestMap tm;

    (*tm)["hello"] = "venachain";
    (*tm).insert(std::make_pair("key", "value"));
    tm->insert(std::make_pair("k", "v"));
  }
  // get
  {
    TestMap tm;
    ASSERT(strcmp("venachain", (*tm)["hello"].c_str()) == 0);
    ASSERT(strcmp("value", (*tm)["key"].c_str()) == 0);
    ASSERT(strcmp("v", (*tm)["k"].c_str()) == 0);
    ASSERT(strcmp("v", tm.get()["k"].c_str()) == 0);

    tm->erase("key");
    ASSERT(tm->find("key") == tm->end());

    tm->clear();
    ASSERT(tm->size() == 0);
  }
}

TEST_CASE(testStorage, array) {
  typedef bcwasm::Array<"test_array"_n, int, 10> TestArray;
  {
    TestArray arr;
    for (auto i = 0; i < 10; i++) {
      (*arr)[i] = i;
    }

    for (auto i = 0; i < 10; i++) {
      ASSERT((*arr)[i] == i);
    }
    ASSERT(arr->at(0) == 0);
    ASSERT(arr->front() == 0);
    ASSERT(arr->back() == 9);
  }

  // get
  {
    TestArray arr;
    for (auto i = 0; i < 10; i++) {
      ASSERT((*arr)[i] == i);
    }
    ASSERT(arr->at(0) == 0);
    ASSERT(arr->front() == 0);
    ASSERT(arr->back() == 9);
  }
}

TEST_CASE(testStorage, tuple) {
  typedef bcwasm::Tuple<"test_tuple"_n, int, std::string, int> TestTuple;
  // set
  {
    TestTuple tpl;
    *tpl = std::make_tuple(10, "test", 9);

    ASSERT(std::get<0>(*tpl) == 10);
    ::clearLog();
    ASSERT(strcmp("test", std::get<1>(*tpl).c_str()) == 0);
    ASSERT(std::get<2>(*tpl) == 9);
  }

  // get
  {
    TestTuple tpl;
    ASSERT(std::get<0>(*tpl) == 10);
    ASSERT(strcmp("test", std::get<1>(*tpl).c_str()) == 0);
    ASSERT(std::get<2>(*tpl) == 9);
  }
}

TEST_CASE(testStorage, deque) {
  typedef bcwasm::Deque<"test_deque"_n, int> TestDeque;
  {
    TestDeque td;
    *td = {1, 2, 3};

    ASSERT((*td)[1] == 2);
    ASSERT(td->at(0) == 1);
    ASSERT(td->front() == 1);
    ASSERT(td->back() == 3);

    td->insert(td->end(), 4);
    ASSERT(td->back() == 4);
    ASSERT(td->size() == 4);
  }

  // get
  {
    TestDeque td;
    ASSERT(td->size() == 4);
    ASSERT((*td)[1] == 2);
    ASSERT(td->at(0) == 1);
    ASSERT(td->front() == 1);
    ASSERT(td->back() == 4);

    td->clear();
    ASSERT(td->size() == 0);
  }
}

UNITTEST_MAIN() {
  RUN_TEST(testStorage, uint8);
  RUN_TEST(testStorage, int8);
  RUN_TEST(testStorage, uint16);
  RUN_TEST(testStorage, int16);
  RUN_TEST(testStorage, uint);
  RUN_TEST(testStorage, int);
  RUN_TEST(testStorage, uint64);
  RUN_TEST(testStorage, int64);
  RUN_TEST(testStorage, string);
  RUN_TEST(testStorage, vector);
  RUN_TEST(testStorage, set);
  RUN_TEST(testStorage, map);
  RUN_TEST(testStorage, array);
  RUN_TEST(testStorage, tuple);
  RUN_TEST(testStorage, deque);
}
