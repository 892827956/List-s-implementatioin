//#pragma once
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
	};													//��������һ���ڵ����  ���ﻹ�ǿ�������ʲô���
	Node* head;//����ͷ���
	Node* tail;//����β�ڵ�
	int size;
	
public:
	/*ԭ���Ľڵ��ֻ�Ǵ����Žڵ��ָ��  ���������init�ĺ����Ϳ�ʼ����һ��ʵ�������*/
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
		const_iterator(Node* p) :current(p) {}				//ת�����캯��
		friend class List<T>;						//����list����Ԫ�࣬����˵��List���������ʹ��const_iterator����
	public:
		const_iterator():current(NULL) {}
		const T& operator*()const { return current->data; }//�����const�������������Ψһ���ڵ����������� Ҳ�Ϳ�ʼ˵���Ҳ�����ͨ����������������ı����current��ֵ
		//����ǰ++ �Ķ���   ����++�Ľ����Ȼ����һ��iterator�Ķ���   �������*this
		const_iterator& operator++()
		{
			current = current->next;
			return *this;
		}
		//��++   �ڶ����++ ��ʱ��  ������صĶ���Ͳ����������� ����Ҫ���ص���һ�������´�����һ������  ���ʹ������  �Ϳ��ܻ������
		const_iterator operator++(int)
		{
			const_iterator old = *this;//�����Ǵ���һ�����󣨸տ�ʼ���ǿգ���������һ����ֵ���캯�����������������ֵ   �Ӷ��ﵽĿ��
			++(*this);//�����е����  ��Ϊ�������ʹ���˸ո����ص�ǰ++  �Ӷ�ʹ��������ƶ�
			return old;
		}
		//ǰ����
		const_iterator& operator--()
		{
			current = current->prev;
			return *this;
		}
		//�󡪡�   ����֮ͬǰ�ĺ�++
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
	//���洴�����iterator��
	class iterator :public const_iterator
	{
	protected:
		iterator(Node* p) :const_iterator(p) {}			//������ֱ���ǵ������const_iteratorde ���캯��  ��ֱ�Ӱ��������   ��ʵ���˼̳�  ��˵���������ʹ������Node�ĳ�Ա
		friend class List<T>;

	public:
		iterator() {}//������캯��  �Ϳ���ֱ�ӵ���
		T& operator *() { return const_iterator::current->data; }		//�� �ҾͲ���retrive()����  �Ҿ������
		const T& operator *() const { return const_iterator::current->data; }/*Ϊʲô������Ҫ���ص��Ǹ����current�����ֵ*/
		iterator& operator ++()
		{
			const_iterator::current = const_iterator::current->next;
			return *this;
		}
		//��++   �ڶ����++ ��ʱ��  ������صĶ���Ͳ����������� ����Ҫ���ص���һ�������´�����һ������  ���ʹ������  �Ϳ��ܻ������
		iterator operator++(int)
		{
			iterator old = *this;//�����Ǵ���һ�����󣨸տ�ʼ���ǿգ���������һ����ֵ���캯�����������������ֵ   �Ӷ��ﵽĿ��
			++(*this);//�����е����  ��Ϊ�������ʹ���˸ո����ص�ǰ++  �Ӷ�ʹ��������ƶ�
			return old;
		}
		//ǰ����
		iterator& operator--()
		{
			const_iterator::current = const_iterator::current->prev;
			return *this;
		}
		//�󡪡�   ����֮ͬǰ�ĺ�++
		iterator operator--(int)
		{
			iterator old = *this;
			--(*this);
			return old;
		}
	};
	/*���������Ҳ����д����   ����������һЩϸ��Ŀǰ����û�и����*/
	//��ʵҲ�ǵ����￪ʼ  List���������ǰ������iterator��const_iterator���������   �Ӷ��������ʹ��  ���ҿ��������������ʹ�õ��������Ϊ��������Ȼ��ͬ����
	/*������ſ�ʼ�����list���ඨ��*/
	List() { Init(); }		//���캯�������Ұѳ�ʼ��������������
	List(const List<T>& l) { Init(); operator =(l); }//����д��������ɶ
	~List() { Clear(); delete head; delete tail; }
	const List& operator=(const List& l);				//����Ϊʲô������ģ��
	iterator Begin() { return iterator(head->next); }		//����Ƚ�����  �൱����ʹ��������������
	const_iterator Begin()const { return const_iterator(head->next); }
	iterator End() { return iterator(tail); }		//����Ƚ�����  �൱����ʹ��������������
	const_iterator End()const { return const_iterator(tail); }

	//��ʼдһд���ӹ��ܵĺ���
	T& Front() { return *Begin(); }		//�������һֱ��д���ĺ���
	const T& Front()const { return *Begin(); }
	T& Back() { return  *(--End()); }
	const T& Back()const { return *(--End()); }//���س���������
	void Push_front(const T& item);//�ײ������item
	void Push_back(const T& item);//β�������item
	void Pop_front();//��ɾ
	void Pop_back();//βɾ
	iterator Erase(iterator itr);//ɾ�����ָʾ�����������ڵ�    ��������Ϊɶ��Ҫ�������������  ����Ҫ����
	iterator Insert(iterator itr, const T& item);//��������ڵ�

	int Size()const { return size; }
	bool Empty() { return size == 0; }
	void Clear() { while (!Empty()) Pop_back(); }		//������Ĺ���  �Ͳ��Ǽ򵥵Ľ����size=0������  ����Ҫȫ�����

};

/*��ʼ����Щ������ʼ����*/
//COPy�������������   ����List������
template<class T>
const List<T>& List<T>::operator = (const List<T>& l)
{
	Clear();		//�������֪����������  Ȼ�����ڽ��в���
	for (const_iterator itr = l.Begin(); itr != l.End(); ++itr)
		Push_back(*itr);
	return *this;
}

//ɾ������    ���ص���ɾ������һ��iterator
template<class T>
typename List<T>::iterator List<T>::Erase(iterator itr)		//������������ⶨ���ʱ�� ����������ʶiterator  ���Ի�������������Դ
{
	//���ɾ���������һ��  ����Ҫ���⴦��
	Node* p = itr.current;
	iterator re(p->next);			//���reʹ���ǽ�Ҫ���ص�һ��ֵ
	p->prev->next = p->next;
	p->next->prev = p->prev;
	delete p;
	size--;		//��һ��������Ҳ���ܲ�����
	return re;
}
template<class T>
typename List<T>::iterator List<T>::Insert(iterator itr, const T& item)
{
	Node* p = itr.current;
	p->prev->next = new Node(item, p->prev, p);			//�������ǰ  �������ֱ��ʹ�ù��캯��
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
	//βɾҪע�����������ǲ���ֱ��ʹ���ҵ�ɾ������
	//ɾ��֮��Ҫȷ��tail��λ��
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
	//��һ����  ����Push_back,Pop_back,Front,Back,Erase
	
	List<int> L;		//����Ϳ�ʼ�����������
	//L.Init();�ڽ��й����ʱ����Ѿ�����ı����һ��ɶ
	cout << "input 10 integers :\n";
	int item;
	for (int i = 0; i < 10; i++)
	{
		cin >> item;
		L.Push_back(item);
	}
	List<int>::iterator itr = L.Begin();
	cout << "after operator ++" << endl;		//�������ڶ���Ԫ��
	cout << *(++itr) << endl;
	cout << "sfter operator --" << endl;
	cout << *(--itr) << endl;
	L.Erase(itr);		//����itr��ʱ����ָ���һ��Ԫ�ء�Ȼ�󷵻�ָ����һ����itr
	cout << "after Erase the last" << endl;
	display_list(L.Begin(), L.End());
	L.Pop_back();
	cout << L.Front() << endl;
	cout << L.Back() << endl;
	//�������
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
/*��ʱ����Ҫ��ʱ���Լ�������һ����*/