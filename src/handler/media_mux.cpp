#include "handler/media_mux.h"

#include "common/srs_kernel_error.h"
#include "utils/json.h"
#include "connection/http_conn.h"
#include "http/http_stack.h"
#include "http/h/http_message.h"
#include "connection/h/media_conn_mgr.h"
#include "rtmp/media_req.h"
#include "handler/media_flv_handler.h"
#include "handler/media_rtc_handler.h"

namespace ma {

MediaHttpServeMux::MediaHttpServeMux() {
  rtc_sevice_ = std::move(std::make_unique<MediaHttpRtcServeMux>());
  flv_sevice_ = std::move(std::make_unique<MediaFlvPlayHandler>());

  g_conn_mgr_.signal_destroy_conn_.connect(this, &MediaHttpServeMux::conn_destroy);
}

MediaHttpServeMux::~MediaHttpServeMux() = default;

srs_error_t MediaHttpServeMux::serve_http(
    IHttpResponseWriter* writer, ISrsHttpMessage* msg) {
  std::string path = msg->path();
  
/*  
  auto found = entry_.find(msg->path());
  if(found == entry_.end()){
    static HttpNotFoundHandler s_hangler_404;
    return s_hangler_404.serve_http(writer, msg);
  }
  return found->second->serve_http(writer, msg);
*/
  if (path == RTC_PUBLISH_PREFIX) {
    rtc_sevice_->serve_http(writer, msg);
  }

  return flv_sevice_->serve_http(writer, msg);
}

srs_error_t MediaHttpServeMux::mount_service(
    std::shared_ptr<MediaSource> s, std::shared_ptr<MediaRequest> r) {
  return flv_sevice_->mount_service(s, r);
}

void MediaHttpServeMux::unmount_service(
    std::shared_ptr<MediaSource> s, std::shared_ptr<MediaRequest> r) {
  flv_sevice_->unmount_service(s, r);
}

void MediaHttpServeMux::conn_destroy(std::shared_ptr<IMediaConnection> conn) {
  flv_sevice_->conn_destroy(conn);
}

std::unique_ptr<IMediaHttpHandler> ServerHandlerFactor::Create() {
  return std::make_unique<MediaHttpServeMux>();
}

}
