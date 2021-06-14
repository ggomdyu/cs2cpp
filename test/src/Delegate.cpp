//#pragma once
//
//#include <memory>
//
//#include <System/Delegate.h>
//
//#include "../Test.h"
//
//CS2CPP_NAMESPACE_BEGIN
//
//class DelegateTest :
//    public Test
//{
//public:
//    void Evaluate() override
//    {
//        std::shared_ptr<int> p;
//        std::shared_ptr<int> p2;
//        std::shared_ptr<int> p3;
//        std::shared_ptr<int> p4;
//        std::shared_ptr<int> p5;
//        std::shared_ptr<int> p6;
//
//        {
//            Delegate<void()> d;
//            Delegate<void()> d2 = d;
//            Delegate<void()> d3 = std::move(d);
//        }
//        {
//            auto temp = Delegate([p, p2, p3, p4, p5, p6]() { return 1; });
//            auto temp2 = Delegate([p, p2, p3, p4, p5, p6]() { return 2; });
//            auto temp3 = temp;
//            assert(temp() == 1);
//            assert(temp2() == 2);
//            assert(temp3() == 1);
//            temp = temp2;
//            assert(temp() == 2);
//            temp2 = std::move(temp3);
//            assert(temp2() == 1);
//        }
//        {
//            auto temp = Delegate([]() { return 1; });
//            auto temp2 = Delegate([]() { return 2; });
//            auto temp3 = temp;
//            assert(temp() == 1);
//            assert(temp2() == 2);
//            assert(temp3() == 1);
//            temp = temp2;
//            assert(temp() == 2);
//            temp2 = std::move(temp3);
//            assert(temp2() == 1);
//        }
//        {
//            auto temp = Delegate([]() { return 1; });
//            auto temp2 = Delegate([p, p2, p3, p4, p5, p6]() { return 2; });
//            auto temp3 = temp;
//            temp2 = std::move(temp);
//            assert(temp2() == 1);
//        }
//        {
//            auto temp = Delegate([]() { return 1; });
//            auto temp2 = Delegate([p, p2, p3, p4, p5, p6]() { return 2; });
//            temp = std::move(temp2);
//        }
//        {
//            auto temp = Delegate([]() { return 1; });
//            auto temp2 = Delegate([p, p2, p3, p4, p5, p6]() { return 2; });
//            temp = temp2;
//        }
//        {
//            auto temp = Delegate([]() { return 1; });
//            auto temp2 = Delegate([p, p2, p3, p4, p5, p6]() { return 2; });
//            temp2 = temp;
//        }
//        {
//            auto temp = Delegate([p, p2, p3, p4, p5, p6]() { return 1; });
//            auto temp2 = Delegate([]() { return 2; });
//            auto temp3 = temp;
//            assert(temp() == 1);
//            assert(temp2() == 2);
//            assert(temp3() == 1);
//            temp = temp2;
//            assert(temp() == 2);
//            temp2 = std::move(temp3);
//            assert(temp2() == 1);
//        }
//    }
//};
//
//CS2CPP_NAMESPACE_END