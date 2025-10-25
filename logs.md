1. 目前的loaddata和storedata是没有问题的，只是将保存的数据放到了D:\project_test\qt5\build\Desktop_Qt_6_9_3_MinGW_64_bit-Debug\bin\data_storage中
2. localtime()的使用问题
    不能调用两次，如果多个线程同时调用，会互相覆盖结果,这是C标准库的历史遗留问题