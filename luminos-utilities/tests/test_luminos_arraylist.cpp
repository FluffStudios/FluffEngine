#include "test_luminos_arraylist.h"
#include "common.h"

void ArraylistTest::TestDefaultConstructor()
{
	arraylist<int> test_list;
	bool result = (test_list.size() == 0);
	ASSERT_TRUE(result)
}

void ArraylistTest::TestConstructorSize()
{
	arraylist<int> test_list(5);
	bool result = (test_list.size() == 0);
	ASSERT_TRUE(result)
}

void ArraylistTest::TestConstructorInitializer()
{
	arraylist<int> test_list {
		3, 2, -1
	};

	bool result = (test_list.size() == 3) && (test_list.data()[0] == 3) && (test_list.data()[1] == 2) && (test_list.data()[2] == -1);
	ASSERT_TRUE(result)
}

void ArraylistTest::TestConstructorInitializer2()
{
	arraylist<int> test_list
	{
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
		10, 11, 12, 13, 14, 15, 16, 17, 18, 19
	};

	bool result = (test_list.size() == 20) && (test_list.data()[19] == 19);
	ASSERT_TRUE(result)
}

void ArraylistTest::TestInsertCopy()
{
	arraylist<int> test_list
	{
		0, 1, 2
	};
	test_list.insert(3);
	test_list.insert(4);
	auto result = (test_list.size() == 5) && (test_list.data()[4] == 4);
	ASSERT_TRUE(result)
}

void ArraylistTest::TestInsertCopyWithExpansion()
{
	arraylist<int> test_list(2);
	test_list.insert(0);
	test_list.insert(1);
	test_list.insert(2);
	test_list.insert(3);
	auto result = (test_list.size() == 4) && (test_list.data()[3] == 3);
	ASSERT_TRUE(result)
}

void ArraylistTest::TestInsertMove()
{
	arraylist<int> test_list
	{
		0, 1, 2
	};
	test_list.insert(std::move(3));
	test_list.insert(std::move(4));
	auto result = (test_list.size() == 5) && (test_list.data()[3] == 3) && (test_list.data()[4] == 4);
	ASSERT_TRUE(result)
}

void ArraylistTest::TestInsertAtIndex()
{
	arraylist<int> test_list
	{
		0, 1, 2
	};
	test_list.insert(-2, 0);
	test_list.insert(-1, 1);

	auto result = (test_list.size() == 5) && (test_list.data()[4] == 2) && (test_list.data()[2] == 0) && (test_list.data()[0] == -2);
	ASSERT_TRUE(result);
}

void ArraylistTest::TestInsertList()
{
	arraylist<int> test_list
	{
		0, 1, 2
	};

	const arraylist<int> inserted_list
	{
		3, 4, 5
	};

	test_list.insert(inserted_list);
	auto result = (test_list.size() == 6) && (test_list.data()[5] == 5);
	ASSERT_TRUE(result)
}

void ArraylistTest::TestBlankInsertList()
{
	arraylist<int> test_list
	{
		0, 1, 2
	};

	const arraylist<int> inserted_list
	{
		
	};

	test_list.insert(inserted_list);
	auto result = (test_list.size() == 3) && (test_list.data()[2] == 2);
	ASSERT_TRUE(result)
}

void ArraylistTest::TestInsertListAtIndex()
{
	arraylist<int> test_list
	{
		0, 1, 2
	};

	const arraylist<int> inserted_list
	{
		3, 4, 5
	};

	test_list.insert(inserted_list, 1);
	auto result = (test_list.size() == 6) && (test_list.data()[5] == 2) && (test_list.data()[3] == 5);
	ASSERT_TRUE(result)
}

void ArraylistTest::TestInsertArray()
{
	arraylist<int> test_list
	{
		0, 1, 2
	};

	int temp[3] = {
		3, 4, 5
	};

	test_list.insert(temp, 3);
	auto result = (test_list.size() == 6) && (test_list.data()[5] == 5);
	ASSERT_TRUE(result)
}

void ArraylistTest::TestInsertArrayAtIndex()
{
	arraylist<int> test_list
	{
		0, 1, 2
	};

	int temp[3] = {
		3, 4, 5
	};

	test_list.insert(temp, 3, 1);
	auto result = (test_list.size() == 6) && (test_list.data()[5] == 2);
	ASSERT_TRUE(result)
}

void ArraylistTest::TestRemove()
{
	arraylist<int> test_list
	{
		0, 1, 2
	};

	const auto i = test_list.remove(static_cast<size_t>(1));
	auto result = (test_list.size() == 2) && (test_list.data()[1] == 2) && (i == 1);
	ASSERT_TRUE(result)
}

void ArraylistTest::TestRemoveValue()
{
	arraylist<int> test_list
	{
		0, 1, 2
	};
	const auto val = test_list.remove(1);
	auto result = (test_list.size() == 2) && val;
	ASSERT_TRUE(result)
}

void ArraylistTest::TestRemoveValue2()
{
	arraylist<int> test_list
	{
		0, 1, 2
	};
	const auto val = test_list.remove(3);
	auto result = (test_list.size() == 3) && !val;
	ASSERT_TRUE(result)
}

void ArraylistTest::TestClear()
{
	arraylist<int> test_list
	{
		0, 1, 2
	};
	test_list.clear();
	ASSERT_TRUE(test_list.size() == 0);
}

void ArraylistTest::TestEmpty()
{
	arraylist<int> test_list
	{
		0, 1, 2
	};
	const auto empty = test_list.empty();
	test_list.clear();
	ASSERT_TRUE(test_list.size() == 0 && !empty);
}

void ArraylistTest::TestData()
{
	arraylist<int> test_list
	{
		0, 1, 2
	};

	ASSERT_TRUE(test_list.data()[0] == 0 && test_list.data()[1] == 1 && test_list.data()[2] == 2)
}

void ArraylistTest::TestAccessOperator()
{
	arraylist<int> test_list
	{
		0, 1, 2
	};

	ASSERT_TRUE(test_list[0] == 0 && test_list[1] == 1 && test_list[2] == 2)
}

void ArraylistTest::TestEmplace()
{
	arraylist<int> test_list 
	{
		0, 1, 2
	};

	test_list.emplace(2, 3);
	ASSERT_TRUE(test_list.size() == 4)
}