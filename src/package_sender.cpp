#include "package_sender.hxx"

void PackageSender::push_package(Package&& p) {
    sending_buffer_ = std::move(p);
}

void PackageSender::send_package() {
    if (!sending_buffer_) return;

    IPackageReceiver* receiver = receiver_preferences_.choose_receiver();
    if (receiver) {
        receiver->receive_package(std::move(*sending_buffer_));
        sending_buffer_.reset();
    }
}

std::optional<Package>& PackageSender::get_sending_buffer() {
    return sending_buffer_;
}
const std::optional<Package>& PackageSender::get_sending_buffer() const {
    return sending_buffer_;
}