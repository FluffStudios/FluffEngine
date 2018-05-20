#pragma once

#include <arraylist.h>

using namespace luminos;

class ArraylistTest
{
public:
	static void TestDefaultConstructor();
	static void TestConstructorSize();
	static void TestConstructorInitializer();
	static void TestConstructorInitializer2();
	static void TestInsertCopy();
	static void TestInsertCopyWithExpansion();
	static void TestInsertMove();
	static void TestInsertAtIndex();
	static void TestInsertList();
	static void TestBlankInsertList();
	static void TestInsertListAtIndex();
	static void TestInsertArray();
	static void TestInsertArrayAtIndex();
	static void TestRemove();
	static void TestRemoveValue();
	static void TestRemoveValue2();
	static void TestClear();
	static void TestEmpty();
	static void TestData();
	static void TestAccessOperator();
	static void TestEmplace();
};