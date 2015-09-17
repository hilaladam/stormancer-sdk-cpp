#pragma once
#include "headers.h"
#include "Packet.h"

namespace Stormancer
{
	/// System request context
	class RequestContext
	{
	public:
	
		/*! Constructor
		\param packet The packet.
		*/
		RequestContext(Packet_ptr packet);
		
		/// Destructor.
		virtual ~RequestContext();

	public:
		Packet_ptr packet();
		bytestream* inputStream();
		
		/// Returns the request completed state.
		bool isComplete();
		
		/*! Sends a response to the request.
		\param writer A method used to write in the request stream.
		*/
		void send(std::function<void(bytestream*)> writer);
		
		/// Completes the system request.
		void complete();
		
		/*! Completes the system request with an error.
		\param writer A method for writing the error.
		*/
		void error(std::function<void(bytestream*)> writer);

	private:
		byte _requestId[2];
		
		/// Packet that initiated the request
		Packet_ptr _packet = nullptr;
		
		/// Stream exposing the request parameters.
		bytestream* _stream = nullptr;
		
		bool _didSendValues = false;
		bool _isComplete;
	};
};
