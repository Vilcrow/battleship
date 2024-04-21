#ifndef CLIENT_ARRAY_H
#define CLIENT_ARRAY_H

class ClientData;

/**
 * @brief The ClientArray class
 */
class ClientArray {
public:
    /**
     * @brief Constructor
     * @param p_size size of the array
     */
    ClientArray(int p_size);

    /**
     * @brief Destructor
     */
    ~ClientArray();

    /**
     * @brief operator []
     * @param index
     * @return if passed invalid index returns 0
     */
    ClientData* operator[](int index) const;

    /**
     * @brief Adds the client to the array
     * @param p_client pointer to the client data
     * @return
     */
    bool add(ClientData *p_client);

    /**
     * @brief Removes the given client from the array
     * @param p_client pointer to the client data
     */
    void remove(ClientData *p_client);

    /**
     * @brief Returns count of clients in the array
     */
    int get_count() const;

    /**
     * @brief Returns size of the array
     */
    int get_size() const;

    /**
     * @brief Returns true if the array is full, otherwise returns false
     */
    bool isFull() const;

    /**
     * @brief Returns true if the array contains a client pointer,
     * otherwise returns false
     * @param p_client pointer to the client data
     */
    bool contains(const ClientData *p_client) const;
private:
    int count;
    int size;
    ClientData **clients;

    void _create_array(ClientData ***r_clients, int p_size);
    int _find_free_pos();
};

#endif
