#include "test_luminos_arraylist.h"

int array_test()
{
	ArraylistTest::TestDefaultConstructor();
	ArraylistTest::TestConstructorSize();
	ArraylistTest::TestConstructorInitializer();
	ArraylistTest::TestConstructorInitializer2();
	ArraylistTest::TestInsertCopy();
	ArraylistTest::TestInsertCopyWithExpansion();
	ArraylistTest::TestInsertMove();
	ArraylistTest::TestInsertAtIndex();
	ArraylistTest::TestInsertList();
	ArraylistTest::TestBlankInsertList();
	ArraylistTest::TestInsertListAtIndex();
	ArraylistTest::TestInsertArray();
	ArraylistTest::TestInsertArrayAtIndex();
	ArraylistTest::TestRemove();
	ArraylistTest::TestRemoveValue();
	ArraylistTest::TestRemoveValue2();
	ArraylistTest::TestClear();
	ArraylistTest::TestEmpty();
	ArraylistTest::TestData();
	ArraylistTest::TestAccessOperator();
	ArraylistTest::TestEmplace();
	printf_s("Tests Complete!\n");
	system("PAUSE");
	return 0;
}
