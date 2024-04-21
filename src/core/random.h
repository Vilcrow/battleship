#ifndef RANDOM_H
#define RANDOM_H

#define RANDOM Random::instance()

/**
 * @brief The singleton class for getting a random number
 */
class Random {
public:
    /**
     * @brief Returns a pointer to a unique instance of the class
     */
    static Random* instance();

    /**
     * @brief Returns a random number smaller than the passed argument
     * @param less_than_num the upper limit of the range
     */
    int rand(int less_than_num);
protected:
    /**
     * @brief A pointer to a unique instance of the class
     */
    static Random *unique;
private:
    Random();
    ~Random();
};

#endif
