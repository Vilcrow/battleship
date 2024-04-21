#ifndef LIST_H
#define LIST_H

/**
 * @brief Template of pointers list (FIFO)
 */
template<class T>
class List {
public:
    /**
     * @brief Constructor
     */
    List();

    /**
     * @brief Destructor
     */
    ~List();

    /**
     * @brief Inserts item at the end of the list
     * @param item
     */
    void put(T *item);

    /**
     * @brief Removes the top item from the list and returns it.
     * If the list is empty, returns the null pointer
     */
    T* get();

    /**
     * @brief Removes the passed pointer from the list and returns it.
     * If the list doesn't contain the pointer, returns the null pointer
     * @param item the pointer to remove
     */
    T* get(const T *item);

    /**
     * @brief Removes the passed pointer from the list if it exists
     * @param item the pointer to remove
     */
    void remove(T *item);

    /**
     * @brief Clears the list
     */
    void clear();

    /**
     * @brief Returns true if the list contains an occurence of item,
     * otherwise returns false
     * @param item the item that is being checked for
     * @return
     */
    bool contains(const T *item) const;

    /**
     * @brief Returns the number of items in the list
     */
    int get_size() const;

    /**
     * @brief Returns true if the list has size 0, otherwise returns false
     */
    bool is_empty() const;

    /**
     * @brief Returns the item at the index position idx
     * @return
     */
    T* operator[](int idx);

    /**
     * @brief Swaps list other with this list
     */
    void swap(List<T> &other);
private:
    struct ListItem {
        T *item;
        ListItem *next;
    };

    ListItem *first;
    ListItem *last;

    int size;
};

template<class T>
List<T>::List()
    : first(0)
    , last(0)
    , size(0)
{

}

template<class T>
List<T>::~List()
{
    clear();
}

template<class T>
void List<T>::put(T *item)
{
    if(!item) {
        return;
    }

    if(!first) {
        first = new ListItem;
        first->item = item;
        first->next = 0;
        last = first;
    }
    else {
        ListItem *tmp = new ListItem;
        tmp->item = item;
        tmp->next = 0;
        last->next = tmp;
        last = tmp;
    }
    ++size;
}

template<class T>
void List<T>::remove(T *item)
{
    ListItem *tmp = first;
    // Empty list.
    if(!tmp || !item) {
        return;
    } 

    // Need remove the first item.
    if(tmp->item == item) {
        first = first->next;
        if(!first) {
            last = 0;
        }
        delete tmp->item;
        delete tmp;
        --size;
        return;
    }

    ListItem *next = tmp->next;
    while(next) {
        if(next->item == item) {
            tmp->next = next->next;
            if(next == last) {
                last = tmp;
            }
            delete next->item;
            delete next;
            --size;
            return;
        }
        tmp = next;
        next = tmp->next;
    }
}
 
template<class T>
void List<T>::clear()
{
    ListItem *tmp;

    while(first) {
        tmp = first;
        first = first->next;
        delete tmp->item;
        delete tmp;
    }
    last = 0;
    size = 0;
}

template<class T>
T* List<T>::get()
{
    if(size == 0) {
        return 0;
    }

    if(first == last) {
        T *ret = first->item;
        delete first;
        first = 0;
        last = 0;
        --size;
        return ret;
    }
    else {
        ListItem *tmp = first;
        T *ret = first->item;
        first = first->next;
        --size;
        delete tmp;
        return ret;
    }
}

template<class T>
T* List<T>::get(const T *item)
{
    ListItem *tmp = first;

    // Empty list or the null pointer.
    if(!item || !tmp) {
        return 0;
    } 

    // Need remove the first item.
    if(tmp->item == item) {
        first = first->next;
        if(!first) {
            last = 0;
        }
        T *ret = tmp->item;
        delete tmp;
        --size;
        return ret;
    }

    ListItem *next = tmp->next;
    while(next) {
        if(next->item == item) {
            tmp->next = next->next;
            T *ret = next->item;
            if(next == last) {
                last = tmp;
            }
            delete next;
            --size;
            return ret;
        }
        tmp = next;
        next = tmp->next;
    }
    return 0;
}

template<class T>
bool List<T>::contains(const T *item) const
{
    ListItem *tmp = first;
    while(tmp) {
        if(tmp->item == item) {
            return true;
        }
        tmp = tmp->next;
    }
    return false;
}

template<class T>
int List<T>::get_size() const
{
    return size;
}

template<class T>
bool List<T>::is_empty() const
{
    return get_size() == 0;
}

template<class T>
T* List<T>::operator[](int idx)
{
    if(idx < 0 || idx >= size) {
        return 0;
    }

    T *ret = 0;
    int i = 0;
    ListItem *tmp = first;
    for(; tmp; tmp = tmp->next) {
        if(i == idx) {
            ret = tmp->item;
            break;
        }
        ++i;
    }
    return ret;
}

template<class T>
void List<T>::swap(List<T> &other)
{
    ListItem *tmp_first = first;
    first = other.first;
    other.first = tmp_first;

    ListItem *tmp_last = last;
    last = other.last;
    other.last = tmp_last;

    int tmp_size = size;
    size = other.size;
    other.size = tmp_size;
}

#endif
