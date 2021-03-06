../protobuf3/src/protoc -I. --cpp_out=. Hello.proto 
g++ -o writer -I ../protobuf3/src writer.cc Hello.pb.cc ../protobuf3/src/.libs/libprotobuf.a
g++ -o reader -I ../protobuf3/src reader.cc Hello.pb.cc ../protobuf3/src/.libs/libprotobuf.a

../bin/pbstru TEST Hello.proto .
cd source
g++ -o writer -D_TEST_CODEC -I ../include pbstru_comm.c hello.c writer.cc
g++ -o reader -D_TEST_CODEC -I ../include pbstru_comm.c hello.c reader.cc
cd ..
