#pragma once
#include<vector>
#include<any>
#include<map>
#include<iostream>
#include<functional>
#include <tuple>
#include <assert.h>
typedef int CallType;

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
	std::function<void(CallType,Params)> out;
    void in(CallType type, Params params) {
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

	template<typename... Args>
	void call(CallType type,Args... args) {
        Params params;
        gen_params(params, args...);
        out(type, params);
	}
    void call(std::any callback) {
        auto fun = std::any_cast<std::function<void(void)> >(callback);
        fun();
    }
    template<typename... Args>
    void call(std::any callback, Args... args) {
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