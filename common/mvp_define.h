#ifndef MVP_DEFINE_H
#define MVP_DEFINE_H



#define SETPROP(n) if(n!=p){n=p;emit propertyChanged(this);}
#define SETGETPROP(type,n) void set##n(type p){if(F##n!=p){F##n=p;emit propertyChanged(this);}} \
                           type n(){return F##n;}
#define MYPROP(type,n) Q_PROPERTY(type n READ n WRITE set##n DESIGNABLE true ) \
                       protected: \
                       type F##n; \
                       public: \
                       void set##n(const type &p){ \
                            if(F##n!=p){ \
                                F##n=p; \
                                doPropertyChanged();}} \
                       const type &n()const  {return F##n;}

#define MYSTATE(type,n) Q_PROPERTY(type n READ n WRITE set##n DESIGNABLE true STORED false) \
                       protected: \
                       type F##n; \
                       public: \
                       void set##n(const type &p){ \
                            if(F##n!=p){ \
                                F##n=p; \
                                doStateChanged();}} \
                       const type &n()const {return F##n;}

#define MYSTATE_RS(type,n) Q_PROPERTY(type n READ n WRITE set##n DESIGNABLE true STORED false) \
                       public: \
                       void set##n(const type &p); \
                       const type &n()const;
#define MYSTATE_R(type,n) Q_PROPERTY(type n READ n DESIGNABLE true STORED false) \
                       public: \
                       type n()const ;
#define MYSTATE_S(type,n) Q_PROPERTY(type n READ n WRITE set##n DESIGNABLE true STORED false) \
                       protected: \
                       type F##n; \
                       public: \
                       void set##n(const type &p); \
                       const type &n()const {return F##n;}


#endif // MVP_DEFINE_H
