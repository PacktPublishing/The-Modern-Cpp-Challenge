#include <iostream>
#include <string>
#include <string_view>
#include <fstream>

#include "curl_easy.h"
#include "curl_exception.h"
#include "curl_header.h"

#include "json.hpp"

using json = nlohmann::json;

std::vector<uint8_t> load_image(std::string_view filepath)
{
   std::vector<uint8_t> data;

   std::ifstream ifile(filepath.data(), std::ios::binary | std::ios::ate);
   if (ifile.is_open())
   {
      auto size = ifile.tellg();
      ifile.seekg(0, std::ios::beg);

      data.resize(static_cast<size_t>(size));
      ifile.read(reinterpret_cast<char*>(data.data()), size);
   }

   return data;
}

struct face_rectangle
{
   int width = 0;
   int height = 0;
   int left = 0;
   int top = 0;
};

struct face_point
{
   double x = 0;
   double y = 0;
};

struct face_landmarks
{
   face_point pupilLeft;
   face_point pupilRight;
   face_point noseTip;
   face_point mouthLeft;
   face_point mouthRight;
   face_point eyebrowLeftOuter;
   face_point eyebrowLeftInner;
   face_point eyeLeftOuter;
   face_point eyeLeftTop;
   face_point eyeLeftBottom;
   face_point eyeLeftInner;
   face_point eyebrowRightInner;
   face_point eyebrowRightOuter;
   face_point eyeRightInner;
   face_point eyeRightTop;
   face_point eyeRightBottom;
   face_point eyeRightOuter;
   face_point noseRootLeft;
   face_point noseRootRight;
   face_point noseLeftAlarTop;
   face_point noseRightAlarTop;
   face_point noseLeftAlarOutTip;
   face_point noseRightAlarOutTip;
   face_point upperLipTop;
   face_point upperLipBottom;
   face_point underLipTop;
   face_point underLipBottom;
};

struct face_emotion
{
   double anger = 0;
   double contempt = 0;
   double disgust = 0;
   double fear = 0;
   double happiness = 0;
   double neutral = 0;
   double sadness = 0;
   double surprise = 0;
};

struct face_attributes
{
   std::string  gender;
   double       age;
   face_emotion emotion;
};

struct face_info
{
   std::string     faceId;
   face_rectangle  rectangle;
   face_landmarks  landmarks;
   face_attributes attributes;
};

struct face_error
{
   std::string code;
   std::string message;
};

struct face_error_response
{
   face_error error;
};

using face_detect_response = std::vector<face_info>;

void from_json(const json& jdata, face_rectangle& rect)
{
   rect.width = jdata.at("width").get<int>();
   rect.height = jdata.at("height").get<int>();
   rect.top = jdata.at("top").get<int>();
   rect.left = jdata.at("left").get<int>();
}

void from_json(const json& jdata, face_point& point)
{
   point.x = jdata.at("x").get<double>();
   point.y = jdata.at("y").get<double>();
}

void from_json(const json& jdata, face_landmarks& mark)
{
   mark.pupilLeft = jdata.at("pupilLeft");
   mark.pupilRight = jdata.at("pupilRight");
   mark.noseTip = jdata.at("noseTip");
   mark.mouthLeft = jdata.at("mouthLeft");
   mark.mouthRight = jdata.at("mouthRight");
   mark.eyebrowLeftOuter = jdata.at("eyebrowLeftOuter");
   mark.eyebrowLeftInner = jdata.at("eyebrowLeftInner");
   mark.eyeLeftOuter = jdata.at("eyeLeftOuter");
   mark.eyeLeftTop = jdata.at("eyeLeftTop");
   mark.eyeLeftBottom = jdata.at("eyeLeftBottom");
   mark.eyeLeftInner = jdata.at("eyeLeftInner");
   mark.eyebrowRightInner = jdata.at("eyebrowRightInner");
   mark.eyebrowRightOuter = jdata.at("eyebrowRightOuter");
   mark.eyeRightInner = jdata.at("eyeRightInner");
   mark.eyeRightTop = jdata.at("eyeRightTop");
   mark.eyeRightBottom = jdata.at("eyeRightBottom");
   mark.eyeRightOuter = jdata.at("eyeRightOuter");
   mark.noseRootLeft = jdata.at("noseRootLeft");
   mark.noseRootRight = jdata.at("noseRootRight");
   mark.noseLeftAlarTop = jdata.at("noseLeftAlarTop");
   mark.noseRightAlarTop = jdata.at("noseRightAlarTop");
   mark.noseLeftAlarOutTip = jdata.at("noseLeftAlarOutTip");
   mark.noseRightAlarOutTip = jdata.at("noseRightAlarOutTip");
   mark.upperLipTop = jdata.at("upperLipTop");
   mark.upperLipBottom = jdata.at("upperLipBottom");
   mark.underLipTop = jdata.at("underLipTop");
   mark.underLipBottom = jdata.at("underLipBottom");
}

void from_json(const json& jdata, face_emotion& emo)
{
   emo.anger = jdata.at("anger").get<double>();
   emo.contempt = jdata.at("contempt").get<double>();
   emo.disgust = jdata.at("disgust").get<double>();
   emo.fear = jdata.at("fear").get<double>();
   emo.happiness = jdata.at("happiness").get<double>();
   emo.neutral = jdata.at("neutral").get<double>();
   emo.sadness = jdata.at("sadness").get<double>();
   emo.surprise = jdata.at("surprise").get<double>();
}

void from_json(const json& jdata, face_attributes& attr)
{
   attr.age = jdata.at("age").get<double>();
   attr.emotion = jdata.at("emotion");
   attr.gender = jdata.at("gender").get<std::string>();
}

void from_json(const json& jdata, face_info& info)
{
   info.faceId = jdata.at("faceId").get<std::string>();
   info.attributes = jdata.at("faceAttributes");
   info.landmarks = jdata.at("faceLandmarks");
   info.rectangle = jdata.at("faceRectangle");
}

void from_json(const json& jdata, face_error& error)
{
   error.code = jdata.at("code").get<std::string>();
   error.message = jdata.at("message").get<std::string>();
}

void from_json(const json& jdata, face_error_response& response)
{
   response.error = jdata.at("error");
}

class face_manager
{
public:
   face_manager(std::string_view endpoint, 
                std::string_view key) 
      : endpoint(endpoint), app_key(key)
   {}

   face_detect_response detect_from_file(std::string_view path)
   {
      try
      {
         auto data = load_image(path);
         if (!data.empty())
         {
            std::stringstream str;
            curl::curl_ios<std::stringstream> writer(str);
            curl::curl_easy easy(writer);

            curl::curl_header header;
            header.add("Ocp-Apim-Subscription-Key:" + app_key);
            header.add("Content-Type:application/octet-stream");

            auto url = endpoint +
               "/detect"
               "?returnFaceId=true"
               "&returnFaceLandmarks=true"
               "&returnFaceAttributes=age,gender,emotion";

            easy.add<CURLOPT_URL>(url.c_str());
            easy.add<CURLOPT_HTTPHEADER>(header.get());

            easy.add<CURLOPT_POSTFIELDSIZE>(data.size());
            easy.add<CURLOPT_POSTFIELDS>(reinterpret_cast<char*>(data.data()));

            easy.perform();

            auto status = easy.get_info<CURLINFO_RESPONSE_CODE>();

            return parse_detect_response(status.get(), str);
         }
      }
      catch (curl::curl_easy_exception const & error)
      {
         auto errors = error.get_traceback();
         error.print_traceback();
      }
      catch (std::exception const & ex)
      {
         std::cout << ex.what() << std::endl;
      }

      return {};
   }
private:
   face_detect_response parse_detect_response(
      long const status, 
      std::stringstream & str)
   {
      json jdata;
      str >> jdata;

      try
      {
         if (status == 200)
         {
            face_detect_response response = jdata;

            return response;
         }
         else if (status >= 400)
         {
            face_error_response response = jdata;

            std::cout << response.error.code << std::endl
               << response.error.message << std::endl;
         }
      }
      catch (std::exception const & ex) 
      {
         std::cout << ex.what() << std::endl;
      }

      return {};
   }

   std::string endpoint;
   std::string app_key;
};

int main()
{
   face_manager manager(
      "https://westeurope.api.cognitive.microsoft.com/face/v1.0",
      "...(your api key)...");
   
#ifdef _WIN32
   std::string path = R"(res\albert_and_elsa.jpg)";
#else
   std::string path = R"(./res/albert_and_elsa.jpg)";
#endif

   auto results = manager.detect_from_file(path);

   for (auto const & face : results)
   {
      std::cout << "faceId: " << face.faceId << std::endl
                << "age:    " << face.attributes.age << std::endl
                << "gender: " << face.attributes.gender << std::endl
                << "rect:   " << "{" << face.rectangle.left 
                              << "," << face.rectangle.top
                              << "," << face.rectangle.width
                              << "," << face.rectangle.height
                              << "}" << std::endl << std::endl;
   }
}
