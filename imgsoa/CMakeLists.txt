add_library(imgsoa imagesoa.cpp resize_soa.cpp cutfreq.cpp compress.cpp)
target_link_libraries(imgsoa PRIVATE common)
