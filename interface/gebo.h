namespace Gebo {
	enum {
		UNDEF = 0,
	};
	
	enum Common {
		FIRST_HEARTBEAT=1,
		HEARTBEAT,
		GET_MODEL_BY_NAME,
		GET_SINGLETON_BY_NAME, // �õ�����
		GET_INSTANCE_BY_NAME,  //�õ�һ��ʵ��

		GET_MODEL_BY_TYPE,
		GET_SINGLETON_BY_TYPE, // �õ�����
		GET_INSTANCE_BY_TYPE,  //�õ�һ��ʵ��
	};

    enum Interface{
		LAUNCHER=99,
        COMPONENT_BUS = 100,
        FILE=200,
        COMPS=300,
        LUA=400,
    };

	enum Event {

	};

	enum User {

	};
}