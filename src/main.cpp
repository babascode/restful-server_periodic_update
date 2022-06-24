
#include "http/http_server.hpp"
#include "infer/simple_yolo.hpp"
#include <unistd.h>
#include <fstream>

using namespace std;
using namespace cv;

static const char *cocolabels[] = {
	"person", "bicycle", "car", "motorcycle", "airplane",
	"bus", "train", "truck", "boat", "traffic light", "fire hydrant",
	"stop sign", "parking meter", "bench", "bird", "cat", "dog", "horse",
	"sheep", "cow", "elephant", "bear", "zebra", "giraffe", "backpack",
	"umbrella", "handbag", "tie", "suitcase", "frisbee", "skis",
	"snowboard", "sports ball", "kite", "baseball bat", "baseball glove",
	"skateboard", "surfboard", "tennis racket", "bottle", "wine glass",
	"cup", "fork", "knife", "spoon", "bowl", "banana", "apple", "sandwich",
	"orange", "broccoli", "carrot", "hot dog", "pizza", "donut", "cake",
	"chair", "couch", "potted plant", "bed", "dining table", "toilet", "tv",
	"laptop", "mouse", "remote", "keyboard", "cell phone", "microwave",
	"oven", "toaster", "sink", "refrigerator", "book", "clock", "vase",
	"scissors", "teddy bear", "hair drier", "toothbrush"};

class InferInstance
{
public:
	bool startup()
	{

		infer_ = get_infer(SimpleYolo::Type::X, SimpleYolo::Mode::FP32, "yolox_s");
		return infer_ != nullptr;
	}

	bool inference(const Mat &image_input, SimpleYolo::BoxArray &boxarray)
	{

		if (infer_ == nullptr)
		{
			INFOE("Not Initialize.");
			return false;
		}

		if (image_input.empty())
		{
			INFOE("Image is empty.");
			return false;
		}
		boxarray = infer_->commit(image_input).get();
		return true;
	}

private:
	bool requires_model(const string &name)
	{

		auto onnx_file = cv::format("%s_dynamic.onnx", name.c_str());
		if (!iLogger::exists(onnx_file))
		{
			printf("Auto download %s\n", onnx_file.c_str());
			system(cv::format("wget http://zifuture.com:1556/fs/25.shared/%s", onnx_file.c_str()).c_str());
		}

		bool isexists = iLogger::exists(onnx_file);
		if (!isexists)
		{
			printf("Download %s failed\n", onnx_file.c_str());
		}
		return isexists;
	}

	shared_ptr<SimpleYolo::Infer> get_infer(SimpleYolo::Type type, SimpleYolo::Mode mode, const string &model)
	{

		int deviceid = 0;
		auto mode_name = SimpleYolo::mode_string(mode);
		SimpleYolo::set_device(deviceid);

		const char *name = model.c_str();
		printf("===================== test %s %s %s ==================================\n", SimpleYolo::type_name(type), mode_name, name);

		if (!requires_model(name))
			return nullptr;

		string onnx_file = cv::format("%s_dynamic.onnx", name);
		string model_file = cv::format("%s_dynamic.%s.trtmodel", name, mode_name);
		int test_batch_size = 16;

		if (!iLogger::exists(model_file))
		{
			SimpleYolo::compile(
				mode, type,		 // FP32、FP16、INT8
				test_batch_size, // max batch size
				onnx_file,		 // source
				model_file,		 // save to
				1 << 30,
				"inference");
		}
		return SimpleYolo::create_infer(model_file, type, deviceid, 0.25f, 0.5f);
	}

private:
	shared_ptr<SimpleYolo::Infer> infer_;
};

class LogicalController : public Controller
{
	SetupController(LogicalController);

public:
	bool startup();

public:
	DefRequestMapping(getCustom); // DefRequestMapping实际就是一个add_router
	DefRequestMapping(getReturn);
	DefRequestMapping(getBinary);
	DefRequestMapping(getFile);
	DefRequestMapping(putBase64Image);
	DefRequestMapping(detectBase64Image);
	DefRequestMapping(putVideo);
	DefRequestMapping(detectVideo);
	// DefRequestMapping(UsrLogin);
	// DefRequestMapping(UsrRegister);
	// DefRequestMapping(downloadVideo);

private:
	shared_ptr<InferInstance> infer_instance_;
	Json::Value detectImage(cv::Mat image_data);
};

Json::Value LogicalController::putVideo(const Json::Value &param)
{
	auto session = get_current_session(); // 返回一个会话用于处理链接。好像只能同时开启32个会话，因为每个会话由一个线程维护，而我们只开启了32个线程。

	// 获取视频开始位置
	string body = session->request.body;
	int i1 = body.find("\r\n");
	i1 = body.find("\r\n", i1 + 2) + 4;

	// std::cout<<session->request.body.substr(i1,1000)<<std::endl;
	iLogger::save_file("base_decode.mp4", session->request.body.substr(i1)); // 保存mp4视频
	std::cout << "已保存MP4视频" << std::endl;

	return success();
}

Json::Value LogicalController::detectImage(cv::Mat image_data)
{
	// auto image = cv::imdecode(image_data, 1);
	if (image_data.empty())
		return failure("Image is empty");

	SimpleYolo::BoxArray boxarray;
	if (!this->infer_instance_->inference(image_data, boxarray))
		return failure("Server error1");

	Json::Value boxarray_json(Json::arrayValue);
	for (auto &box : boxarray)
	{
		Json::Value item(Json::objectValue);
		item["left"] = box.left;
		item["top"] = box.top;
		item["right"] = box.right;
		item["bottom"] = box.bottom;
		item["confidence"] = box.confidence;
		item["class_label"] = box.class_label;
		item["class_name"] = cocolabels[box.class_label];
		boxarray_json.append(item);
	}
	return success(boxarray_json);
}

// Json::Value LogicalController::downloadVideo(const Json::Value &param)
// {
// }

Json::Value LogicalController::detectVideo(const Json::Value &param)
{
	auto session = get_current_session(); // 返回一个会话用于处理链接。好像只能同时开启32个会话，因为每个会话由一个线程维护，而我们只开启了32个线程。

	// 获取视频开始位置
	string body = session->request.body;
	int i1 = body.find("\r\n");
	i1 = body.find("\r\n\r\n", i1) + 4;

	// std::cout << i1 << std::endl;
	// std::cout<<session->request.body.substr(i1,1000)<<std::endl;
	iLogger::save_file("base_decode.mp4", session->request.body.substr(i1)); // 保存mp4视频
	// std::cout << session->request.body.substr(i1, 1000) << std::endl;

	std::cout << "已保存MP4视频" << std::endl;

	// 循环提取出视频中图片进行处理，并放回到视频中
	cv::VideoCapture cap;
	cap.open("base_decode.mp4");

	/*获取视频fps*/
	double rate = cap.get(cv::CAP_PROP_FPS);
	/*获取视频帧的尺寸*/
	int width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
	int height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
	auto fourcc = cap.get(cv::CAP_PROP_FOURCC);

	// std::cout << fourcc << std::endl;
	/*根据打开视频的参数初始化输出视频格式*/
	// cv::VideoWriter w_cap("re_video.avi", VideoWriter::fourcc('M', 'P', '4', '2'), rate, cv::Size(width, height));
	// cv::VideoWriter w_cap("re_video.mp4", VideoWriter::fourcc('m', 'p', '4', 'v'), rate, cv::Size(width, height));

	cv::VideoWriter w_cap("re_video.mp4", fourcc, rate, cv::Size(width, height));
	/*自定义输出视频的尺寸，需要将读取的视频帧尺寸进行变换，下文使用的resize函数完成*/
	// cv::VideoWriter w_cap("re_video.avi", CV_FOURCC('M', 'J', 'P', 'G'), rate, cv::Size(your.width, your.height));

	if (!cap.isOpened())
		return 0;

	cv::Mat frame;
	Json::Value resultJson;
	while (1)
	{
		cap >> frame;
		if (frame.empty())
			break;

		resultJson = detectImage(frame);

		// 如果框，就给给frame画框
		for (auto &box : resultJson["data"])
		{
			cv::rectangle(frame, cv::Point(box["left"].asInt(), box["top"].asInt()), cv::Point(box["right"].asInt(), box["bottom"].asInt()), cv::Scalar(0, 0, 255));
			// cv::rectangle(frame, cv::Point(int(box["left"]), 400), cv::Point(450, 450), cv::Scalar(0, 0, 255));
			// std::cout << typeid(box["left"].asInt()).name() << std::endl;
			// cap << frame;
		}
		// 替换相应帧
		w_cap.write(frame);

		cv::imwrite("base_decode.jpg", frame);
		// cv::imshow("hello", frame);
		//  cv::waitKey(50);
	}

	Json::Value resultVideoJson;
	resultVideoJson["url"] = "http://127.0.0.1:8090/static/re_video.mp4";
	return resultVideoJson;
}

Json::Value LogicalController::putBase64Image(const Json::Value &param)
{

	/**
	 * 注意，这个函数的调用，请用工具（postman）以提交body的方式(raw)提交base64数据
	 * 才能够在request.body中拿到对应的base64，并正确解码后储存
	 * 1. 可以在网页上提交一个图片文件，并使用postman进行body-raw提交，例如网址是：https://base64.us/，选择页面下面的“选择文件”按钮
	 * 2. 去掉生成的base64数据前缀：data:image/png;base64,。保证是纯base64数据输入
	 *   这是一个图像的base64案例：iVBORw0KGgoAAAANSUhEUgAAABwAAAAcCAYAAAByDd+UAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAADsQAAA7EAZUrDhsAAABLSURBVEhLY2RY9OI/Ax0BE5SmG6DIh/8DJKAswoBxwwswTXcfjlpIdTBqIdXBqIVUB8O/8B61kOpg1EKqg1ELqQ5GLaQ6oLOFDAwA5z0K0dyTzgcAAAAASUVORK5CYII=
	 *   提交后能看到是个天蓝色的背景加上右上角有黄色的正方形
	 */

	auto session = get_current_session(); // 返回一个会话用于处理链接。好像只能同时开启32个会话，因为每个会话由一个线程维护，而我们只开启了32个线程。

	// iLogger::save_file("base_decode.mp4", session->request.body);
	// std::cout << session->request.body.substr(0, 1000) << std::endl;

	string body = session->request.body;
	// int i1 = body.find("\r\n");
	// i1 = body.find("\r\n",i1+2)+4;

	// std::cout<<session->request.body.substr(i1,1000)<<std::endl;       // 找到视频的开头
	// iLogger::save_file("base_decode.mp4", session->request.body.substr(i1)); // 保存mp4视频
	//  auto image_data = iLogger::base64_decode(session->request.body);
	//  iLogger::save_file("base_decode.jpg", image_data);

	return success();
}

Json::Value LogicalController::detectBase64Image(const Json::Value &param)
{

	auto session = get_current_session();

	// std::cout << session->request.body.substr(0, 1000) << std::endl;

	auto image_data = iLogger::base64_decode(session->request.body);

	// std::cout<< session->request.url << std::endl;

	iLogger::save_file("base_decode.jpg", image_data);
	// iLogger::save_file("base_decode.mp4", session->request.body);

	if (image_data.empty())
		return failure("Image is required");

	auto image = cv::imdecode(image_data, 1);
	if (image_data.empty())
		return failure("Image is empty");

	SimpleYolo::BoxArray boxarray;
	if (!this->infer_instance_->inference(image, boxarray))
		return failure("Server error1");

	Json::Value boxarray_json(Json::arrayValue);
	for (auto &box : boxarray)
	{
		Json::Value item(Json::objectValue);
		item["left"] = box.left;
		item["top"] = box.top;
		item["right"] = box.right;
		item["bottom"] = box.bottom;
		item["confidence"] = box.confidence;
		item["class_label"] = box.class_label;
		item["class_name"] = cocolabels[box.class_label];
		boxarray_json.append(item);
	}
	return success(boxarray_json);
}

Json::Value LogicalController::getCustom(const Json::Value &param)
{

	auto session = get_current_session();
	const char *output = "hello http server";
	session->response.write_binary(output, strlen(output));
	session->response.set_header("Content-Type", "text/plain");
	return success();
}

Json::Value LogicalController::getReturn(const Json::Value &param)
{

	Json::Value data;
	data["alpha"] = 3.15;
	data["beta"] = 2.56;
	data["name"] = "张三";
	return success(data);
}

Json::Value LogicalController::getBinary(const Json::Value &param)
{

	auto session = get_current_session();
	auto data = iLogger::load_file("img.jpg");
	session->response.write_binary(data.data(), data.size());
	session->response.set_header("Content-Type", "image/jpeg");
	return success();
}

Json::Value LogicalController::getFile(const Json::Value &param)
{

	auto session = get_current_session();
	session->response.write_file("img.jpg");
	return success();
}

bool LogicalController::startup()
{

	infer_instance_.reset(new InferInstance()); // 给指针赋值
	if (!infer_instance_->startup())
	{
		infer_instance_.reset(); // 将指针清零
	}
	return infer_instance_ != nullptr;
}

int test_http(int port = 8090)
{
	// 整体的流程是创建controller和server，然后将controller加入到server中吗？
	// 向server中加入链接，链接传递给controller。具体通信是由controller实现的吗？还是由controller管理各种会话？？

	INFO("Create controller");
	auto logical_controller = make_shared<LogicalController>();
	if (!logical_controller->startup())
	{ // 加载推理器是否成功
		INFOE("Startup controller failed.");
		return -1;
	}

	string address = iLogger::format("0.0.0.0:%d", port); // 就是返回字符串"0.0.0.0:8090"
	INFO("Create http server to: %s", address.c_str());

	// server中进行了什么设置，accept？listen？如何处理多个用户连接
	auto server = createHttpServer(address, 32); // 返回一个HttpServerImpl对象，HttpServerImpl和LogicalController有什么关系
	if (!server)
		return -1;

	INFO("Add controller");													 // 控制器是个什么，内容是什么功能是什么？答：控制器应该是用于管理链接
	server->add_controller("/api", logical_controller);						 // 所有URL以api开头的，都使用控制器logical_controller
																			 // 我只见过ip+端口的监听，那么ip+端口+/字符串是怎么处理的？答：用一个map数组保存<链接,处理函数>
	server->add_controller("/", create_redirect_access_controller("./web")); // create_redirect_access_controller返回一个controller子类
	server->add_controller("/static", create_file_access_controller("./"));	 // create_file_access_controller返回一个controller子类
	INFO("Access url: http://%s", address.c_str());

	INFO(
		"\n"
		"访问如下地址即可看到效果:\n"
		"1. http://%s/api/getCustom              使用自定义写出内容作为response\n"
		"2. http://%s/api/getReturn              使用函数返回值中的json作为response\n"
		"3. http://%s/api/getBinary              使用自定义写出二进制数据作为response\n"
		"4. http://%s/api/getFile                使用自定义写出文件路径作为response\n"
		"5. http://%s/api/putBase64Image         通过提交base64图像数据进行解码后储存\n"
		"6. http://%s/static/img.jpg             直接访问静态文件处理的controller，具体请看函数说明\n"
		"7. http://%s                            访问web页面，vue开发的",
		address.c_str(), address.c_str(), address.c_str(), address.c_str(), address.c_str(), address.c_str(), address.c_str());

	INFO("按下Ctrl + C结束程序");
	return iLogger::while_loop();
}

int main(int argc, char **argv)
{

	return test_http();
}
