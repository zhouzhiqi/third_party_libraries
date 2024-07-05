//
// Created by zhou on 24-4-6.
//

#ifndef BASIC_KITS_MQTT_CLIENT_H
#define BASIC_KITS_MQTT_CLIENT_H

#include <atomic>
#include <thread>
#include <string>
#include <map>
#include <mqtt/async_client.h>
#include "bounded_buffer.hpp"

namespace zzq {

    typedef int (mqtt_topic_handler)(const std::string &topic_str, const std::string &payload_str);

    class mqtt_client {
    public:
        mqtt_client()
            : _ring_buff(10), _mqtt_client_ptr(nullptr), _is_running(false),
              _handlers_by_topics(), _thread_receiver(), _thread_runner() {};

        ~mqtt_client() = default;

        static std::shared_ptr<mqtt_client> register_client(const std::string &client_name);
        static std::shared_ptr<mqtt_client> get_client_ptr(const std::string &client_name);

        int init(
            const std::string &server_addr, const std::string &client_id,
            const std::string &user_name, const std::string &user_pw
        );

        int subscribe_topics(
            const std::map<std::string, mqtt_topic_handler *> &topics_handlers
        );

        int publish(const std::string &topic, const std::string &message);

        int start();

        int stop();

        bool is_running();

    private:
        void receiver();

        void runner();

        static std::map<std::string, std::shared_ptr<mqtt_client>> S_name_to_client_ptr;
        bounded_buffer<mqtt::const_message_ptr> _ring_buff;
        std::shared_ptr<mqtt::async_client> _mqtt_client_ptr;
        std::atomic_bool _is_running;
        std::map<std::string, mqtt_topic_handler *> _handlers_by_topics;
        std::thread _thread_receiver;
        std::thread _thread_runner;

    };

} // zzq

#endif //BASIC_KITS_MQTT_CLIENT_H
