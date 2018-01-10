#pragma once
#include<vector>
#include<any>
#include<map>
#include<iostream>
#include<functional>
#include <tuple>
#include <assert.h>
typedef int CallType;
enum DataType
{
    TYPE_INT,
	TYPE_FLOAT,
	TYPE_DOUBLE,
	TYPE_STRING,
	TYPE_PTR
};
class Contain
{
 public:
     DataType type_;
     std::any data_;
     Contain(DataType type, std::any data):type_(type),data_(data) {}
    ~Contain(){}
    DataType type() { return type_; }
    std::any data() { return data_; }
    void get(void * resualt) {
        switch (type_)
        {
        case TYPE_INT:
            *(int *)resualt = std::any_cast<int>(data_);
            break;
        case TYPE_FLOAT:
            *(float *)resualt = std::any_cast<float>(data_);
            break;
        case TYPE_DOUBLE:
            *(double *)resualt = std::any_cast<double>(data_);
            break;
        case TYPE_STRING:
            *(std::string *)resualt = std::any_cast<std::string>(data_);
            break;
        case TYPE_PTR:
            *(void **)resualt = std::any_cast<void *>(data_);
            break;
        default:
            break;
        }
    }
};

class Result
{
    bool success;
    Contain data;
};

class Params {
private:
    std::vector<std::any> data_;
public:
    Params() {}
    ~Params() {}
    size_t size() const { return data_.size(); }
    void clear() { data_.clear(); }
    bool empty() { return data_.empty(); }
    template <typename T>
    bool push_param(T t) {
        std::any value = t;
        data_.push_back(value);
        return true;
    }

    template <typename T>
    bool pop_param(T &t) {
        assert(0 < size());
        t = std::any_cast<T>(data_.back());
        data_.pop_back();
        return true;
    }

    template <typename T>
    bool get_param(unsigned int i, T &t) const {
        assert(i < size());
        t = std::any_cast<T>(data_.at(i));
        return true;
    }

    template <typename T>
    T get_param(unsigned int i) {
        assert(i < size());
        return std::any_cast<T>(data_.at(i));
    }
};

////////////////////////////////////////////////////////////////
template <typename Head, typename... Args>
class GenParamTuple {
public:
    static inline std::tuple<Head, Args...> Run(int index, Params &p) {
        std::tuple<Args...> t = GenParamTuple<Args...>::Run(index + 1, p);
        Head pn;
        p.get_param(index, pn);
        return std::tuple_cat(std::make_tuple(pn), t);
    }
};

template <typename Last>
class GenParamTuple<Last> {
public:
    static inline std::tuple<Last> Run(int index, Params &p) {
        Last pn;
        p.get_param(index, pn);
        return std::make_tuple(pn);
    }
};

struct Callable {
    Callable() {}
    ~Callable() {}
    virtual void operator()(Params &p) = 0;
};
typedef std::shared_ptr<Callable> CallablePtr;

template <typename... Args>
struct CallObject : public Callable {
    std::function<void(Args...)> fun;
    void operator()(Params &p) {
        auto t_p = GenParamTuple<Args...>::Run(0, p);
        std::apply(fun, t_p);
    }
};

template <>
struct CallObject<void> : public Callable {
    std::function<void(void)> fun;
    void operator()(Params &p) { fun(); }
};
/////////////////////////////////////////////////////////////////////////////////////
class  Component
{
public:
	std::function<int(CallType,Params)> out;
    void in(CallType type, Params params) {
		auto itr = function.find(type);
		if (itr == function.end()) return;

        CallablePtr call = function[type];
        (*call)(params);
    }
public:
	Component(){}
	virtual ~Component(){}

    virtual void init() {}
    virtual void destory(){}
public:
    std::map<CallType, CallablePtr> function;
    bool register_call(CallType type, std::function<void(void)> any_fun) {
        CallObject<void> *new_call = new CallObject<void>;
        new_call->fun = any_fun;
        function[type] = CallablePtr(new_call);
        return true;
    }
    //template <typename... Args>
    //bool register_call(CallType type,std::function<void(Args...)> any_fun) {
    //    CallObject<Args...> *new_call = new CallObject<Args...>;
    //    new_call->fun = any_fun;
    //    function[type] = CallablePtr(new_call);
    //    return true;
    //}

    template <typename... Args>
    bool register_call(CallType type,std::function<void(Args...)> any_fun) {
        CallObject<Args...> *new_call = new CallObject<Args...>;
        new_call->fun = any_fun;
        function[type] = CallablePtr(new_call);
        return true;
    }

    void call(CallType type) {
        Params params;
        out(type, params);
    }
	template<typename... Args>
	void call(CallType type,Args... args) {
        Params params;
        gen_params(params, args...);
        out(type, params);
	}
    void invoke(std::any callback) {
        auto fun = std::any_cast<std::function<void(void)> >(callback);
        fun();
    }
    template<typename... Args>
    void invoke(std::any callback, Args... args) {
        auto fun = std::any_cast<std::function<void(Args...)> >(callback);
        fun(args...);
    }

    void gen_params(Params &params) { return; }
    template <class T, class... Args>
    void gen_params(Params &params, T head, Args... rest) {
        params.push_param(head);
        gen_params(params, rest...);
    }
};

//////////////////////////////////////////////////////////////////////////////////////
//__declspec(dllexport)  Component *create();
//__declspec(dllexport)  Component *instance();

#define EXPORT_COMPONENT(NAME) \
extern "C" _declspec(dllexport)  Component *create() { \
    return new NAME;    \
}   \
extern "C" _declspec(dllexport)  Component *instance() { \
    static auto ins = new NAME; \
    return ins; \
} 
