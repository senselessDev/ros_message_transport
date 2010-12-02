#ifndef UDPMULTI_TRANSPORT_PUBLISHER_H
#define UDPMULTI_TRANSPORT_PUBLISHER_H

#include <boost/asio.hpp>


#include <roslib/Header.h>
#include <message_transport/simple_publisher_plugin.h>
#include <udpmulti_transport/UDPMultHeader.h>

namespace udpmulti_transport {

    static const unsigned int MAX_UDP_PACKET_SIZE = 8092;

	class UDPMultiPublisherImpl 
	{
		public:
            UDPMultiPublisherImpl();
			virtual ~UDPMultiPublisherImpl();

            void setNodeHandle(ros::NodeHandle & nh) {
                nh_ = nh;
            }

			void initialise(const std::string & topicname);
			uint32_t getPort() const;
			const std::string & getMulticastAddress() const;

			UDPMultHeader getUDPHeader() const;

			void multicast(const ros::Message & message,uint32_t datasize=0);
		protected:
			
			uint32_t port_;
			std::string multicast_address_;
            boost::asio::io_service io_service_;
            boost::asio::ip::udp::endpoint *endpoint_;
            boost::asio::ip::udp::socket *socket_;
            ros::NodeHandle nh_;

	};

	template <class Base>
	class UDPMultiPublisher : 
		public message_transport::SimplePublisherPlugin<Base,udpmulti_transport::UDPMultHeader>
	{
		public:
			UDPMultiPublisher() : 
                message_transport::SimplePublisherPlugin<Base,udpmulti_transport::UDPMultHeader>(true), // force latch
				tlast_header_(-1), first_run_(true) {}
			virtual ~UDPMultiPublisher() {}

			virtual std::string getTransportName() const
			{
				return "udpmulti";
			}
		protected:
			virtual void postAdvertiseInit() {
                impl.setNodeHandle(this->getNodeHandle());
            }
			virtual void connectCallback(const ros::SingleSubscriberPublisher& pub) {
                ROS_INFO("Received connection request");
            }
			virtual void disconnectCallback(const ros::SingleSubscriberPublisher& pub) {
                ROS_INFO("Received disconnection request");
            }
            
			virtual void publish(const Base& message,
					const typename message_transport::SimplePublisherPlugin<Base,udpmulti_transport::UDPMultHeader>::PublishFn& publish_fn) const {
				
                uint32_t datasize;
                if (first_run_) {
                    impl.initialise(this->getTopic());
                    this->publishInternal(impl.getUDPHeader());
                    first_run_ = false;
                }

				
				datasize = ros::serialization::serializationLength(message);
				if (datasize > MAX_UDP_PACKET_SIZE) {
					ROS_ERROR("This type of message is too big (%d bytes) for UDP (max %d bytes)",
							datasize, MAX_UDP_PACKET_SIZE);
					return;
				}
				impl.multicast(message,datasize);
			}

			mutable UDPMultiPublisherImpl impl;
			uint32_t port_;
			double tlast_header_;
            mutable bool first_run_;
			
	};


} //namespace udpmulti_transport


#endif // UDPMULTI_TRANSPORT_PUBLISHER_H
