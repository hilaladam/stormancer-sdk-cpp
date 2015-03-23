/*
 *  Copyright (c) 2014, Oculus VR, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant 
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef __ROOMS_ERROR_CODES_H
#define __ROOMS_ERROR_CODES_H

namespace RakNet
{

enum RoomsErrorCode
{
	REC_SUCCESS,
	REC_USERNAME_IS_EMPTY, // Generic error, when the requester of an operation did not define his username
	REC_NOT_LOGGED_IN, // Generic error, when the requester of an operation is not logged in
	REC_ADD_TO_ROOM_NO_RESERVED_OR_PUBLIC,
	REC_ADD_TO_ROOM_NO_PUBLIC,
	REC_ADD_TO_ROOM_NO_SPECTATOR,
	REC_ADD_TO_ROOM_ALREADY_IN_THIS_ROOM,
	REC_ADD_TO_ROOM_ALREADY_IN_ANOTHER_ROOM,
	REC_ADD_TO_ROOM_KICKED_OUT_OF_ROOM,
	REC_CHANGE_MEMBER_TYPE_NO_SLOTS,
	REC_SEARCH_BY_FILTER_UNKNOWN_TITLE,
	REC_JOIN_BY_FILTER_UNKNOWN_TITLE,
	REC_JOIN_BY_FILTER_NO_ROOMS,
	REC_JOIN_BY_FILTER_CURRENTLY_IN_A_ROOM,
	REC_JOIN_BY_FILTER_CURRENTLY_IN_QUICK_JOIN,
	REC_JOIN_BY_FILTER_CANNOT_JOIN_AS_MODERATOR,
	REC_JOIN_BY_FILTER_ROOM_LOCKED,
	REC_JOIN_BY_FILTER_BANNED,
	REC_JOIN_BY_FILTER_NO_SLOTS,
	REC_JOIN_BY_QUICK_JOIN_CANNOT_JOIN_AS_MODERATOR,
	REC_JOIN_BY_QUICK_JOIN_ROOM_LOCKED,
	REC_JOIN_BY_QUICK_JOIN_BANNED,
	REC_JOIN_BY_QUICK_JOIN_NO_SLOTS,
	REC_ADD_TO_QUICK_JOIN_CURRENTLY_IN_A_ROOM,
	REC_ADD_TO_QUICK_JOIN_UNKNOWN_TITLE,
	REC_ADD_TO_QUICK_JOIN_ALREADY_THERE,
	REC_ADD_TO_QUICK_JOIN_INVALID_TIMEOUT_TOO_LOW,
	REC_ADD_TO_QUICK_JOIN_INVALID_TIMEOUT_TOO_HIGH,
	REC_ADD_TO_QUICK_JOIN_MINIMUM_SLOTS_TOO_LOW,
	REC_ADD_TO_QUICK_JOIN_MINIMUM_SLOTS_TOO_HIGH,
	REC_REMOVE_FROM_QUICK_UNKNOWN_TITLE,
	REC_REMOVE_FROM_QUICK_JOIN_NOT_THERE,
	REC_CREATE_ROOM_UNKNOWN_TITLE,
	REC_CREATE_ROOM_CURRENTLY_IN_QUICK_JOIN,
	REC_CREATE_ROOM_CURRENTLY_IN_A_ROOM,
	REC_ROOM_CREATION_PARAMETERS_EMPTY_ROOM_NAME,
	REC_ROOM_CREATION_PARAMETERS_RESERVED_QUICK_JOIN_ROOM_NAME,
	REC_ROOM_CREATION_PARAMETERS_ROOM_NAME_HAS_PROFANITY,
	REC_ROOM_CREATION_PARAMETERS_ROOM_NAME_IN_USE,
	REC_ROOM_CREATION_PARAMETERS_NO_PLAYABLE_SLOTS,
	REC_SET_ROOM_PROPERTIES_UNKNOWN_ROOM,
	REC_LEAVE_ROOM_UNKNOWN_ROOM_ID,
	REC_LEAVE_ROOM_CURRENTLY_IN_QUICK_JOIN,
	REC_LEAVE_ROOM_NOT_IN_ROOM,
	REC_ENTER_ROOM_UNKNOWN_TITLE,
	REC_ENTER_ROOM_CURRENTLY_IN_QUICK_JOIN,
	REC_ENTER_ROOM_CURRENTLY_IN_A_ROOM,
	REC_PROCESS_QUICK_JOINS_UNKNOWN_TITLE,
	REC_ROOM_QUERY_TOO_MANY_QUERIES,
	REC_ROOM_QUERY_INVALID_QUERIES_POINTER,
	REC_SEND_INVITE_UNKNOWN_ROOM_ID,
	REC_SEND_INVITE_INVITEE_ALREADY_INVITED,
	REC_SEND_INVITE_CANNOT_PERFORM_ON_SELF,
	REC_SEND_INVITE_INVITOR_ONLY_MODERATOR_CAN_INVITE, // INVITE_MODE_MODERATOR_ONLY
	REC_SEND_INVITE_INVITOR_LACK_INVITE_PERMISSIONS, // Any other INVITE_MODE
	REC_SEND_INVITE_INVITOR_NOT_IN_ROOM,
	REC_SEND_INVITE_NO_SLOTS,
	REC_SEND_INVITE_INVITEE_ALREADY_IN_THIS_ROOM,
	REC_SEND_INVITE_INVITEE_BANNED,
	REC_SEND_INVITE_RECIPIENT_NOT_ONLINE,
	REC_SEND_INVITE_ROOM_LOCKED,
	REC_ACCEPT_INVITE_UNKNOWN_ROOM_ID,
	REC_ACCEPT_INVITE_CURRENTLY_IN_A_ROOM,
	REC_ACCEPT_INVITE_CURRENTLY_IN_QUICK_JOIN,
	REC_ACCEPT_INVITE_BANNED,
	REC_ACCEPT_INVITE_NO_SLOTS,
	REC_ACCEPT_INVITE_ROOM_LOCKED,
	REC_ACCEPT_INVITE_NO_SUCH_INVITE,
	REC_SLOTS_VALIDATION_NO_PLAYABLE_SLOTS,
	REC_SLOTS_VALIDATION_NEGATIVE_PUBLIC_SLOTS,
	REC_SLOTS_VALIDATION_NEGATIVE_RESERVED_SLOTS,
	REC_SLOTS_VALIDATION_NEGATIVE_SPECTATOR_SLOTS,
	REC_START_SPECTATING_UNKNOWN_ROOM_ID,
	REC_START_SPECTATING_ALREADY_SPECTATING,
	REC_START_SPECTATING_NO_SPECTATOR_SLOTS_AVAILABLE,
	REC_START_SPECTATING_NOT_IN_ROOM,
	REC_START_SPECTATING_REASSIGN_MODERATOR_BEFORE_SPECTATE,
	REC_START_SPECTATING_ROOM_LOCKED,
	REC_STOP_SPECTATING_UNKNOWN_ROOM_ID,
	REC_STOP_SPECTATING_NOT_IN_ROOM,
	REC_STOP_SPECTATING_NOT_CURRENTLY_SPECTATING,
	REC_STOP_SPECTATING_NO_SLOTS,
	REC_STOP_SPECTATING_ROOM_LOCKED,
	REC_GRANT_MODERATOR_UNKNOWN_ROOM_ID,
	REC_GRANT_MODERATOR_NEW_MODERATOR_NOT_ONLINE,
	REC_GRANT_MODERATOR_NOT_IN_ROOM,
	REC_GRANT_MODERATOR_NEW_MODERATOR_NOT_IN_ROOM,
	REC_GRANT_MODERATOR_CANNOT_PERFORM_ON_SELF,
	REC_GRANT_MODERATOR_MUST_BE_MODERATOR_TO_GRANT_MODERATOR,
	REC_GRANT_MODERATOR_NEW_MODERATOR_NOT_IN_PLAYABLE_SLOT,
	REC_CHANGE_SLOT_COUNTS_UNKNOWN_ROOM_ID,
	REC_CHANGE_SLOT_COUNTS_NOT_IN_ROOM,
	REC_CHANGE_SLOT_COUNTS_MUST_BE_MODERATOR,
	REC_SET_CUSTOM_ROOM_PROPERTIES_UNKNOWN_ROOM_ID,
	REC_SET_CUSTOM_ROOM_PROPERTIES_CONTAINS_DEFAULT_COLUMNS,
	REC_SET_CUSTOM_ROOM_PROPERTIES_NOT_IN_ROOM,
	REC_SET_CUSTOM_ROOM_PROPERTIES_MUST_BE_MODERATOR,
	REC_GET_ROOM_PROPERTIES_EMPTY_ROOM_NAME_AND_NOT_IN_A_ROOM,
	REC_GET_ROOM_PROPERTIES_UNKNOWN_ROOM_NAME,
	REC_CHANGE_ROOM_NAME_UNKNOWN_ROOM_ID,
	REC_CHANGE_ROOM_NAME_NOT_IN_ROOM,
	REC_CHANGE_ROOM_NAME_MUST_BE_MODERATOR,
	REC_CHANGE_ROOM_NAME_HAS_PROFANITY,
	REC_CHANGE_ROOM_NAME_EMPTY_ROOM_NAME,
	REC_CHANGE_ROOM_NAME_NAME_ALREADY_IN_USE,
	REC_SET_HIDDEN_FROM_SEARCHES_UNKNOWN_ROOM_ID,
	REC_SET_HIDDEN_FROM_SEARCHES_NOT_IN_ROOM,
	REC_SET_HIDDEN_FROM_SEARCHES_MUST_BE_MODERATOR,
	REC_SET_DESTROY_ON_MODERATOR_LEAVE_UNKNOWN_ROOM_ID,
	REC_SET_DESTROY_ON_MODERATOR_LEAVE_NOT_IN_ROOM,
	REC_SET_DESTROY_ON_MODERATOR_LEAVE_MUST_BE_MODERATOR,
	REC_SET_READY_STATUS_UNKNOWN_ROOM_ID,
	REC_SET_READY_STATUS_NOT_IN_ROOM,
	REC_SET_READY_STATUS_NOT_IN_PLAYABLE_SLOT,
	REC_SET_READY_STATUS_AUTO_LOCK_ALL_PLAYERS_READY,
	REC_GET_READY_STATUS_NOT_IN_ROOM,
	REC_GET_READY_STATUS_UNKNOWN_ROOM_ID,
	REC_SET_ROOM_LOCK_STATE_UNKNOWN_ROOM_ID,
	REC_SET_ROOM_LOCK_STATE_NOT_IN_ROOM,
	REC_SET_ROOM_LOCK_STATE_MUST_BE_MODERATOR,
	REC_SET_ROOM_LOCK_STATE_BAD_ENUMERATION_VALUE,
	REC_GET_ROOM_LOCK_STATE_UNKNOWN_ROOM_ID,
	REC_GET_ROOM_LOCK_STATE_NOT_IN_ROOM,
	REC_ARE_ALL_MEMBERS_READY_UNKNOWN_ROOM_ID,
	REC_ARE_ALL_MEMBERS_READY_NOT_IN_ROOM,
	REC_KICK_MEMBER_UNKNOWN_ROOM_ID,
	REC_KICK_MEMBER_NOT_IN_ROOM,
	REC_KICK_MEMBER_TARGET_NOT_ONLINE,
	REC_KICK_MEMBER_TARGET_NOT_IN_YOUR_ROOM,
	REC_KICK_MEMBER_MUST_BE_MODERATOR,
	REC_KICK_MEMBER_CANNOT_PERFORM_ON_SELF,
	REC_GET_KICK_REASON_UNKNOWN_ROOM_ID,
	REC_GET_KICK_REASON_NOT_KICKED,
	REC_REMOVE_USER_NOT_IN_ROOM,
	REC_ADD_TITLE_ALREADY_IN_USE,
	REC_UNBAN_MEMBER_UNKNOWN_ROOM_ID,
	REC_UNBAN_MEMBER_NOT_IN_ROOM,
	REC_UNBAN_MEMBER_MUST_BE_MODERATOR,
	REC_UNBAN_MEMBER_NOT_BANNED,
	REC_GET_BAN_REASON_UNKNOWN_ROOM_ID,
	REC_GET_BAN_REASON_NOT_BANNED,
	REC_CHANGE_HANDLE_NEW_HANDLE_IN_USE,
	REC_CHANGE_HANDLE_CONTAINS_PROFANITY,
	REC_CHAT_USER_NOT_IN_ROOM,
	REC_CHAT_RECIPIENT_NOT_ONLINE,
	REC_CHAT_RECIPIENT_NOT_IN_ANY_ROOM,
	REC_CHAT_RECIPIENT_NOT_IN_YOUR_ROOM,
	REC_BITSTREAM_USER_NOT_IN_ROOM,
	REC_BITSTREAM_RECIPIENT_NOT_ONLINE,
	REC_BITSTREAM_RECIPIENT_NOT_IN_ANY_ROOM,
	REC_BITSTREAM_RECIPIENT_NOT_IN_YOUR_ROOM,

	ROOMS_ERROR_CODES_COUNT
};

struct RoomsErrorCodeDescription
{
	RoomsErrorCode errorCode;
	const char *enumDesc;
	const char *englishDesc;

	static const char *ToEnglish(RoomsErrorCode result);
	static const char *ToEnum(RoomsErrorCode result);
	static void Validate(void);
};

}

#endif