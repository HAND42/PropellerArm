#ifndef MODEL_TIME_H
#define MODEL_TIME_H

class model_time
{
    public:
        static model_time* getInstance();

        void init(void);

        int time_desired_get(void);
        void time_desired_set(const int);

    private:
        model_time(void);
        ~model_time(void);

        int time_desired;
};

#endif
