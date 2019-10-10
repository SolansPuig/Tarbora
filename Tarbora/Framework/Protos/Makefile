PROTOC = protoc
GRPC_CPP_PLUGIN = grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`

all: messages.pb.cc messages.grpc.pb.cc

%.pb.cc: %.proto
	$(PROTOC) -I . --cpp_out=. --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<

%.grpc.pb.cc: %.proto
	$(PROTOC) -I . --cpp_out=. $<
