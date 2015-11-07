#基于served的http服务器Demo

###一、served服务器：
该服务器使用C++11与boost1.56开发，编译时需要gcc4.8版本以及至少boost1.53版本。
该服务器已经在我的github的仓库中，只阅读使用了该服务器，未贡献代码

###二、Demo的功能与实现

1. 该Demo先在attri文件（已在此目录下）中读取文件中的json字符串。每个json串中包含一个商品的名字与链接，以及对该商品的各种分类。   
2. 在这里使用tags与topics两种分类，建立两个UCB类，来保存每个客户对tags与topics的喜好。  
3. 再使用unorderedmap分别建立一个tags/topics与std::vector<std::string>联系。vector中保存的就是有着相同tags或者topics分类的商品json字符串
4. 建立推荐handler，输入参数中包含key=tags/topics，number=要推荐的数量
5. 建立反馈handler，输入key=tags/topics,tags的名字或者topics的名字，以及更新的value值。
6. 通过网页访问该url，可以看到推荐的结果与预想的相同


##详细介绍
更加详细的Demo介绍请转至我的博客：   
http://blog.csdn.net/yw8355507/article/details/49206395