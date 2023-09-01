//
// Copyright (c) 2013-2021 Winlin
//
// SPDX-License-Identifier: MIT
//
// This file is borrowed from srs with some modifications.


#ifndef SRS_KERNEL_CONSTS_HPP
#define SRS_KERNEL_CONSTS_HPP

#include <chrono>

#define PERF_MW_SLEEP 100 //ms

/**
 * how many msgs can be send entirely.
 * for play clients to get msgs then totally send out.
 * for the mw sleep set to 1800, the msgs is about 133.
 * @remark, recomment to 128.
 */
constexpr int PERF_MW_MSGS = 10;

/**
 * how many chunk stream to cache, [0, N].
 * to imporove about 10% performance when chunk size small, and 5% for large chunk.
 * @see https://github.com/ossrs/srs/issues/249
 * @remark 0 to disable the chunk stream cache.
 */
constexpr int SRS_PERF_CHUNK_STREAM_CACHE = 16;

// Default port of rtmp
#define SRS_CONSTS_RTMP_DEFAULT_PORT 1935

// Default http listen port.
#define SRS_DEFAULT_HTTP_PORT 80

// Default https listen port.
#define SRS_DEFAULT_HTTPS_PORT 443

// Default Redis listen port.
#define SRS_DEFAULT_REDIS_PORT 6379

///////////////////////////////////////////////////////////
// RTMP consts values
///////////////////////////////////////////////////////////
// Default vhost of rtmp
#define SRS_CONSTS_RTMP_DEFAULT_VHOST "__defaultVhost__"
#define SRS_CONSTS_RTMP_DEFAULT_APP "__defaultApp__"

// The default chunk size for system.
#define SRS_CONSTS_RTMP_SRS_CHUNK_SIZE 60000
// 6. Chunking, RTMP protocol default chunk size.
#define SRS_CONSTS_RTMP_PROTOCOL_CHUNK_SIZE 128

// 6. Chunking
// The chunk size is configurable. It can be set using a control
// message(Set Chunk Size) as described in section 7.1. The maximum
// chunk size can be 65536 bytes and minimum 128 bytes. Larger values
// reduce CPU usage, but also commit to larger writes that can delay
// other content on lower bandwidth connections. Smaller chunks are not
// good for high-bit rate streaming. Chunk size is maintained
// independently for each direction.
#define SRS_CONSTS_RTMP_MIN_CHUNK_SIZE 128
#define SRS_CONSTS_RTMP_MAX_CHUNK_SIZE 65536


// The following is the timeout for rtmp protocol,
// to avoid death connection.

// The common io timeout, for connect, recv or send.
// TODO: FIXME: Maybe change to smaller value, such as 3s?
#define SRS_CONSTS_RTMP_TIMEOUT (30 * SRS_UTIME_SECONDS)

// The timeout to wait for client control message,
// if timeout, we generally ignore and send the data to client,
// generally, it's the pulse time for data seding.
// @remark, recomment to 500ms.
#define SRS_CONSTS_RTMP_PULSE (500 * UTIME_MILLISECONDS)

// The max rtmp header size:
//     1bytes basic header,
//     11bytes message header,
//     4bytes timestamp header,
// that is, 1+11+4=16bytes.
#define SRS_CONSTS_RTMP_MAX_FMT0_HEADER_SIZE 16
// The max rtmp header size:
//     1bytes basic header,
//     4bytes timestamp header,
// that is, 1+4=5bytes.
// always use fmt0 as cache.
#define SRS_CONSTS_RTMP_MAX_FMT3_HEADER_SIZE 5

// For performance
#define SRS_CONSTS_IOVS_MAX (PERF_MW_MSGS * 2)
#define SRS_CONSTS_C0C3_HEADERS_MAX (PERF_MW_MSGS * 32)

///////////////////////////////////////////////////////////
// SRS consts values
///////////////////////////////////////////////////////////
#define SRS_CONSTS_NULL_FILE "/dev/null"
#define SRS_CONSTS_LOCALHOST "127.0.0.1"
#define SRS_CONSTS_LOOPBACK "0.0.0.0"
#define SRS_CONSTS_LOOPBACK6 "::"

// The signal defines.
// To reload the config file and apply new config.
#define SRS_SIGNAL_RELOAD SIGHUP
// Reopen the log file.
#define SRS_SIGNAL_REOPEN_LOG SIGUSR1
// For gracefully upgrade, start new SRS and gracefully quit old one.
// @see https://github.com/ossrs/srs/issues/1579
// TODO: Not implemented.
#define SRS_SIGNAL_UPGRADE SIGUSR2
// The signal for srs to fast quit, do essential dispose then exit.
#define SRS_SIGNAL_FAST_QUIT SIGTERM
// The signal for srs to gracefully quit, do carefully dispose then exit.
// @see https://github.com/ossrs/srs/issues/1579
#define SRS_SIGNAL_GRACEFULLY_QUIT SIGQUIT

// The application level signals.
// Persistence the config in memory to config file.
// @see https://github.com/ossrs/srs/issues/319#issuecomment-134993922
// @remark we actually don't handle the signal for it's not a valid os signal.
#define SRS_SIGNAL_PERSISTENCE_CONFIG 1000

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// The log consts values
///////////////////////////////////////////////////////////
// Downloading speed-up, play to edge, ingest from origin
#define SRS_CONSTS_LOG_EDGE_PLAY "EIG"
// Uploading speed-up, publish to edge, foward to origin
#define SRS_CONSTS_LOG_EDGE_PUBLISH "EPB"
// The edge/origin forwarder.
#define SRS_CONSTS_LOG_FOWARDER "FWR"
// Play stream on edge/origin.
#define SRS_CONSTS_LOG_PLAY "PLA"
// Client publish to edge/origin
#define SRS_CONSTS_LOG_CLIENT_PUBLISH "CPB"
// The web/flash publish to edge/origin
#define SRS_CONSTS_LOG_WEB_PUBLISH "WPB"
// Ingester for edge(play)/origin
#define SRS_CONSTS_LOG_INGESTER "IGS"
// The hls log id.
#define SRS_CONSTS_LOG_HLS "HLS"
// The encoder log id.
#define SRS_CONSTS_LOG_ENCODER "ENC"
// The http stream log id.
#define SRS_CONSTS_LOG_HTTP_STREAM "HTS"
// The http stream cache log id.
#define SRS_CONSTS_LOG_HTTP_STREAM_CACHE "HTC"
// The stream caster log id.
#define SRS_CONSTS_LOG_STREAM_CASTER "SCS"
// The nginx exec log id.
#define SRS_CONSTS_LOG_EXEC "EXE"
// The rtc.
#define SRS_CONSTS_LOG_RTC "RTC"
// The gb28181 stream log id.
#define SRS_CONSTS_LOG_GB28181_CASTER "GBS"

///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// HTTP/HLS consts values
///////////////////////////////////////////////////////////
// @see hls-m3u8-draft-pantos-http-live-streaming-12.pdf, page 4
// Lines are terminated by either a single LF character or a CR
// character followed by an LF character.
// CR             = <US-ASCII CR, carriage return (13)>
#define SRS_CONSTS_CR '\r' // 0x0D
// LF             = <US-ASCII LF, linefeed (10)>
#define SRS_CONSTS_LF '\n' // 0x0A
// SP             = <US-ASCII SP, space>
#define SRS_CONSTS_SP ' ' // 0x20
// SE             = <US-ASCII SE, semicolon>
#define SRS_CONSTS_SE ';' // 0x3b
// LB             = <US-ASCII SE, left-brace>
#define SRS_CONSTS_LB '{' // 0x7b
// RB             = <US-ASCII SE, right-brace>
#define SRS_CONSTS_RB '}' // 0x7d

///////////////////////////////////////////////////////////
// HTTP consts values
///////////////////////////////////////////////////////////
// The default http port.
#define SRS_CONSTS_HTTP_DEFAULT_PORT 80
// The linux path seprator
#define SRS_CONSTS_HTTP_PATH_SEP '/'
// Query string seprator
#define SRS_CONSTS_HTTP_QUERY_SEP '?'

// The default recv timeout.
#define SRS_HTTP_RECV_TIMEOUT (15 * SRS_UTIME_SECONDS)

// 6.1.1 Status Code and Reason Phrase
#define SRS_CONSTS_HTTP_Continue                       100
#define SRS_CONSTS_HTTP_SwitchingProtocols             101
#define SRS_CONSTS_HTTP_OK                             200
#define SRS_CONSTS_HTTP_Created                        201
#define SRS_CONSTS_HTTP_Accepted                       202
#define SRS_CONSTS_HTTP_NonAuthoritativeInformation    203
#define SRS_CONSTS_HTTP_NoContent                      204
#define SRS_CONSTS_HTTP_ResetContent                   205
#define SRS_CONSTS_HTTP_PartialContent                 206
#define SRS_CONSTS_HTTP_MultipleChoices                300
#define SRS_CONSTS_HTTP_MovedPermanently               301
#define SRS_CONSTS_HTTP_Found                          302
#define SRS_CONSTS_HTTP_SeeOther                       303
#define SRS_CONSTS_HTTP_NotModified                    304
#define SRS_CONSTS_HTTP_UseProxy                       305
#define SRS_CONSTS_HTTP_TemporaryRedirect              307
#define SRS_CONSTS_HTTP_BadRequest                     400
#define SRS_CONSTS_HTTP_Unauthorized                   401
#define SRS_CONSTS_HTTP_PaymentRequired                402
#define SRS_CONSTS_HTTP_Forbidden                      403
#define SRS_CONSTS_HTTP_NotFound                       404
#define SRS_CONSTS_HTTP_MethodNotAllowed               405
#define SRS_CONSTS_HTTP_NotAcceptable                  406
#define SRS_CONSTS_HTTP_ProxyAuthenticationRequired    407
#define SRS_CONSTS_HTTP_RequestTimeout                 408
#define SRS_CONSTS_HTTP_Conflict                       409
#define SRS_CONSTS_HTTP_Gone                           410
#define SRS_CONSTS_HTTP_LengthRequired                 411
#define SRS_CONSTS_HTTP_PreconditionFailed             412
#define SRS_CONSTS_HTTP_RequestEntityTooLarge          413
#define SRS_CONSTS_HTTP_RequestURITooLarge             414
#define SRS_CONSTS_HTTP_UnsupportedMediaType           415
#define SRS_CONSTS_HTTP_RequestedRangeNotSatisfiable   416
#define SRS_CONSTS_HTTP_ExpectationFailed              417
#define SRS_CONSTS_HTTP_InternalServerError            500
#define SRS_CONSTS_HTTP_NotImplemented                 501
#define SRS_CONSTS_HTTP_BadGateway                     502
#define SRS_CONSTS_HTTP_ServiceUnavailable             503
#define SRS_CONSTS_HTTP_GatewayTimeout                 504
#define SRS_CONSTS_HTTP_HTTPVersionNotSupported        505

#define SRS_CONSTS_HTTP_Continue_str                           "Continue"
#define SRS_CONSTS_HTTP_SwitchingProtocols_str                 "Switching Protocols"
#define SRS_CONSTS_HTTP_OK_str                                 "OK"
#define SRS_CONSTS_HTTP_Created_str                            "Created"
#define SRS_CONSTS_HTTP_Accepted_str                           "Accepted"
#define SRS_CONSTS_HTTP_NonAuthoritativeInformation_str        "Non Authoritative Information"
#define SRS_CONSTS_HTTP_NoContent_str                          "No Content"
#define SRS_CONSTS_HTTP_ResetContent_str                       "Reset Content"
#define SRS_CONSTS_HTTP_PartialContent_str                     "Partial Content"
#define SRS_CONSTS_HTTP_MultipleChoices_str                    "Multiple Choices"
#define SRS_CONSTS_HTTP_MovedPermanently_str                   "Moved Permanently"
#define SRS_CONSTS_HTTP_Found_str                              "Found"
#define SRS_CONSTS_HTTP_SeeOther_str                           "See Other"
#define SRS_CONSTS_HTTP_NotModified_str                        "Not Modified"
#define SRS_CONSTS_HTTP_UseProxy_str                           "Use Proxy"
#define SRS_CONSTS_HTTP_TemporaryRedirect_str                  "Temporary Redirect"
#define SRS_CONSTS_HTTP_BadRequest_str                         "Bad Request"
#define SRS_CONSTS_HTTP_Unauthorized_str                       "Unauthorized"
#define SRS_CONSTS_HTTP_PaymentRequired_str                    "Payment Required"
#define SRS_CONSTS_HTTP_Forbidden_str                          "Forbidden"
#define SRS_CONSTS_HTTP_NotFound_str                           "Not Found"
#define SRS_CONSTS_HTTP_MethodNotAllowed_str                   "Method Not Allowed"
#define SRS_CONSTS_HTTP_NotAcceptable_str                      "Not Acceptable"
#define SRS_CONSTS_HTTP_ProxyAuthenticationRequired_str        "Proxy Authentication Required"
#define SRS_CONSTS_HTTP_RequestTimeout_str                     "Request Timeout"
#define SRS_CONSTS_HTTP_Conflict_str                           "Conflict"
#define SRS_CONSTS_HTTP_Gone_str                               "Gone"
#define SRS_CONSTS_HTTP_LengthRequired_str                     "Length Required"
#define SRS_CONSTS_HTTP_PreconditionFailed_str                 "Precondition Failed"
#define SRS_CONSTS_HTTP_RequestEntityTooLarge_str              "Request Entity Too Large"
#define SRS_CONSTS_HTTP_RequestURITooLarge_str                 "Request URI Too Large"
#define SRS_CONSTS_HTTP_UnsupportedMediaType_str               "Unsupported Media Type"
#define SRS_CONSTS_HTTP_RequestedRangeNotSatisfiable_str       "Requested Range Not Satisfiable"
#define SRS_CONSTS_HTTP_ExpectationFailed_str                  "Expectation Failed"
#define SRS_CONSTS_HTTP_InternalServerError_str                "Internal Server Error"
#define SRS_CONSTS_HTTP_NotImplemented_str                     "Not Implemented"
#define SRS_CONSTS_HTTP_BadGateway_str                         "Bad Gateway"
#define SRS_CONSTS_HTTP_ServiceUnavailable_str                 "Service Unavailable"
#define SRS_CONSTS_HTTP_GatewayTimeout_str                     "Gateway Timeout"
#define SRS_CONSTS_HTTP_HTTPVersionNotSupported_str            "HTTP Version Not Supported"


// HTTP/1.1 defines the sequence CR LF as the end-of-line marker for all
// protocol elements except the entity-body (see appendix 19.3 for
// tolerant applications).
#define SRS_HTTP_CRLF "\r\n" // 0x0D0A
#define SRS_HTTP_CRLFCRLF "\r\n\r\n" // 0x0D0A0D0A

#endif

