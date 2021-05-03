
#include<iostream>
using namespace std;
template<class T>
class List
{
	struct Node
	{
		T data;
		Node* prev, * next;
		Node(const T& d = T(), Node* p = NULL, Node* n = NULL) :data(d), prev(p), next(n) {}
	};													//申明这是一个节点的类  这里还是看不出来什么差别
	Node* head;//虚拟头结点
	Node* tail;//虚拟尾节点
	int size;
	
public:
	/*原来的节点的只是存在着节点的指针  但是在这个init的函数就开始创造一个实体的链表*/
	void Init()
	{
		size = 0;
		head = new Node;
		tail = new Node;
		head->next = tail;
		tail->prev = head;
	}
	class const_iterator
	{
	protected:
		Node* current;
		T& retrieve()const { return current->data; }
		const_iterator(Node* p) :current(p) {}				//转换构造函数
		friend class List<T>;						//声明list是友元类，就是说明List类里面可以使用const_iterator的类
	public:
		const_iterator():current(NULL) {}
		const T& operator*()const { return current->data; }//这里的const是这个常迭代器唯一存在的意义在这里 也就开始说明我不可以通过这个常迭代器而改变这个current的值
		//用于前++ 的定义   由于++的结果依然还是一个iterator的对象   因而返回*this
		const_iterator& operator++()
		{
			current = current->next;
			return *this;
		}
		//后++   在定义后++ 的时候  这个返回的对象就不能是引用了 我需要返回的是一个我重新创建的一个对象  如果使用引用  就可能会出问题
		const_iterator operator++(int)
		{
			const_iterator old = *this;//首先是创建一个对象（刚开始的是空），再利用一个赋值构造函数重载来对这个对象赋值   从而达到目的
			++(*this);//这里有点灵活  因为这里就是使用了刚刚重载的前++  从而使这个往后移动
			return old;
		}
		//前——
		const_iterator& operator--()
		{
			current = current->prev;
			return *this;
		}
		//后——   道理同之前的后++
		const_iterator operator--(int)
		{
			iterator old = *this;
			--(*this);
			return old;
		}
		bool operator ==(const const_iterator& rhs)const
		{
			return current == rhs.current;
		}
		bool operator !=(const const_iterator& rhs)const
		{
			return current != rhs.current;
		}
	};
	//下面创建这个iterator类
	class iterator :public const_iterator
	{
	protected:
		iterator(Node* p) :const_iterator(p) {}			//在这里直接是调用这个const_iteratorde 构造函数  就直接把这个用了   其实用了继承  就说明这个本身就存在这个Node的成员
		friend class List<T>;

	public:
		iterator() {}//这个构造函数  就可以直接调用
		T& operator *() { return const_iterator::current->data; }		//哎 我就不用retrive()函数  我就是玩儿
		const T& operator *() const { return const_iterator::current->data; }/*为什么这里需要返回的是父类的current里面的值*/
		iterator& operator ++()
		{
			const_iterator::current = const_iterator::current->next;
			return *this;
		}
		//后++   在定义后++ 的时候  这个返回的对象就不能是引用了 我需要返回的是一个我重新创建的一个对象  如果使用引用  就可能会出问题
		iterator operator++(int)
		{
			iterator old = *this;//首先是创建一个对象（刚开始的是空），再利用一个赋值构造函数重载来对这个对象赋值   从而达到目的
			++(*this);//这里有点灵活  因为这里就是使用了刚刚重载的前++  从而使这个往后移动
			return old;
		}
		//前——
		iterator& operator--()
		{
			const_iterator::current = const_iterator::current->prev;
			return *this;
		}
		//后——   道理同之前的后++
		iterator operator--(int)
		{
			iterator old = *this;
			--(*this);
			return old;
		}
	};
	/*这个类里面也算是写完了   至于这个类的一些细节目前还是没有搞清楚*/
	//其实也是到这里开始  List的类里面是包含这个iterator和const_iterator的两个类的   从而可以随便使用  对我看来这个两个类在使用的情况上认为是两个截然不同的类
	/*在这里才开始对这个list的类定义*/
	List() { Init(); }		//构造函数里面我把初始化函数放在里面
	List(const List<T>& l) { Init(); operator =(l); }//这样写的意义是啥
	~List() { Clear(); delete head; delete tail; }
	const List& operator=(const List& l);				//这里为什么不带类模板
	iterator Begin() { return iterator(head->next); }		//这里比较巧妙  相当于是使用了无名命对象
	const_iterator Begin()const { return const_iterator(head->next); }
	iterator End() { return iterator(tail); }		//这里比较巧妙  相当于是使用了无名命对象
	const_iterator End()const { return const_iterator(tail); }

	//开始写一写更加功能的函数
	T& Front() { return *Begin(); }		//灵活运用一直的写过的函数
	const T& Front()const { return *Begin(); }
	T& Back() { return  *(--End()); }
	const T& Back()const { return *(--End()); }//返回常量型引用
	void Push_front(const T& item);//首插入这个item
	void Push_back(const T& item);//尾插如这个item
	void Pop_front();//首删
	void Pop_back();//尾删
	iterator Erase(iterator itr);//删除这个指示器上面的这个节点    至于这里为啥需要返回这个迭代器  还需要考虑
	iterator Insert(iterator itr, const T& item);//插入这个节点

	int Size()const { return size; }
	bool Empty() { return size == 0; }
	void Clear() { while (!Empty()) Pop_back(); }		//这个清表的过程  就不是简单的将这个size=0就完了  还需要全部清空

};

/*开始对这些函数开始定义*/
//COPy运算符进行重载   返回List的引用
template<class T>
const List<T>& List<T>::operator = (const List<T>& l)
{
	Clear();		//把这个已知的这个表清空  然后再在进行操作
	for (const_iterator itr = l.Begin(); itr != l.End(); ++itr)
		Push_back(*itr);
	return *this;
}

//删除函数    返回的是删除的下一个iterator
template<class T>
typename List<T>::iterator List<T>::Erase(iterator itr)		//由于这个在类外定义的时候 编译器不认识iterator  所以还必须标明这个来源
{
	//如果删除的是最后一个  则还需要另外处理
	Node* p = itr.current;
	iterator re(p->next);			//这个re使我们将要返回的一个值
	p->prev->next = p->next;
	p->next->prev = p->prev;
	delete p;
	size--;		//这一步在这里也还能不能忘
	return re;
}
template<class T>
typename List<T>::iterator List<T>::Insert(iterator itr, const T& item)
{
	Node* p = itr.current;
	p->prev->next = new Node(item, p->prev, p);			//相较于以前  这里可以直接使用构造函数
	p->prev = p->prev->next;
	size++;
	return iterator(p->prev);
}
template<class T>
void List<T>::Push_back(const T&item)
{
	iterator itr(this->tail);
	Insert(itr, item);
}

template<class T>
void List<T>::Push_front(const T&item)
{
	iterator itr(this->head->naxt);
	Insert(itr, item);
}

template<class T>
void List<T>::Pop_back()
{
	//尾删要注意的这个问题是不能直接使用我的删除函数
	//删除之后要确定tail的位置
	Node * p = tail;
	tail = tail->prev;
	delete p;
	size--;

}
template<class T>
void List<T>::Pop_front()
{
	iterator itr(this->head->next);
	Erase(itr);
}

template<class Iterator>
void display_list(Iterator first, Iterator last);


int main()
{
	//第一部分  测试Push_back,Pop_back,Front,Back,Erase
	
	List<int> L;		//这里就开始创建这个容器
	//L.Init();在进行构造的时候就已经将其改变成了一个啥
	cout << "input 10 integers :\n";
	int item;
	for (int i = 0; i < 10; i++)
	{
		cin >> item;
		L.Push_back(item);
	}
	List<int>::iterator itr = L.Begin();
	cout << "after operator ++" << endl;		//输出这个第二个元素
	cout << *(++itr) << endl;
	cout << "sfter operator --" << endl;
	cout << *(--itr) << endl;
	L.Erase(itr);		//这里itr暂时还是指向第一个元素。然后返回指向下一个的itr
	cout << "after Erase the last" << endl;
	display_list(L.Begin(), L.End());
	L.Pop_back();
	cout << L.Front() << endl;
	cout << L.Back() << endl;
	//邢增语句
	cout << "updata the first and the last" << endl;
	L.Front() = 100;
	L.Back() = 200;
	display_list(L.Begin(), L.End());

	return 0;
}

//template<typename T>
template<class Iterator>
void display_list(Iterator first, Iterator last)
{
	for (; first != last; first++)
	{
		cout << *first << "   "; 
	}
	cout << endl;
}
/*到时候需要花时间自己来做出一个类*/
