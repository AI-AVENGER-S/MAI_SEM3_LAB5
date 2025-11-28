#include "gtest/gtest.h"
#include <iostream>
#include <string>

#include "Memory_resourse.h"
#include "DoublyLinkedList.h" 

constexpr size_t POOL_SIZE = 1024;


TEST(MemoryResourceTest, AllocationAndDeallocation) {
    MemoryResource mr(POOL_SIZE);
    
    void* p1 = mr.allocate(32, alignof(char));
    ASSERT_NE(p1, nullptr);
    
    mr.deallocate(p1, 32, alignof(char));
}

TEST(MemoryResourceTest, ReuseMemoryTest) {
    MemoryResource mr(POOL_SIZE);
    
    void* p_orig = mr.allocate(64, alignof(char));
    
    mr.deallocate(p_orig, 64, alignof(char));
    
    void* p_reused = mr.allocate(64, alignof(char));
    
    ASSERT_EQ(p_orig, p_reused) << "Expected memory reuse, but got different address.";

    mr.deallocate(p_reused, 64, alignof(char));
}

TEST(MemoryResourceTest, OutOfMemoryTest) {
    MemoryResource mr(32); 
    
    ASSERT_THROW({
        mr.allocate(64, alignof(char));
    }, std::bad_alloc) << "Expected bad_alloc when running out of fixed buffer memory.";
}


struct TestStruct {
    int x;
    std::string name;
    TestStruct(int i, const std::string& n) : x(i), name(n) {}
};

TEST(DoublyLinkedListTest, BasicOperations) {
    MemoryResource mr(POOL_SIZE);
    DoublyLinkedList<int> list(&mr);
    
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    auto it = list.begin();
    ASSERT_EQ(*it, 1); ++it;
    ASSERT_EQ(*it, 2); ++it;
    ASSERT_EQ(*it, 3); ++it;
    ASSERT_EQ(it, list.end());
}

TEST(DoublyLinkedListTest, PopBackAndIterator) {
    MemoryResource mr(POOL_SIZE);
    DoublyLinkedList<int> list(&mr);
    
    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    list.pop_back();
    
    auto it = list.begin();
    ASSERT_EQ(*it, 10); 
    
    ++it;
    ASSERT_EQ(*it, 20);
    

    
    --it;
    ASSERT_EQ(*it, 10);
    
    ++it;
    ++it;
    ASSERT_EQ(it, list.end());
}


TEST(DoublyLinkedListTest, ComplexStruct) {
    MemoryResource mr(POOL_SIZE);
    DoublyLinkedList<TestStruct> list(&mr);
    
    list.push_back({5, "Alpha"});
    list.push_back({10, "Beta"});

    auto it = list.begin();
    ASSERT_EQ(it->x, 5);
    ASSERT_EQ(it->name, "Alpha");
    
    ++it;
    ASSERT_EQ(it->x, 10);
    ASSERT_EQ(it->name, "Beta");
}