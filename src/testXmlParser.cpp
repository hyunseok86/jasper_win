#include <iostream>
#include "pugixml.hpp"

int main() {
    pugi::xml_document doc;

    // XML 파싱
    pugi::xml_parse_result result = doc.load_file("your_xml_file.xml");

    if (result) {
        std::cout << "XML 파일을 성공적으로 로드했습니다." << std::endl;

        // 접수통보일시(CCYYMMDDHHMMSS) 추출
        pugi::xml_node issueDateTimeNode = doc.child("wco:Response").child("wco:IssueDateTime");
        std::string issueDateTime = issueDateTimeNode.text().get();

        // 문서형태구분 추출
        pugi::xml_node typeCodeNode = doc.child("wco:Response").child("wco:TypeCode");
        std::string typeCode = typeCodeNode.text().get();

        // 접수 세관/과 추출
        pugi::xml_node declarationOfficeIDNode = doc.child("wco:Response").child("wco:Declaration").child("wco:DeclarationOfficeID");
        std::string declarationOfficeID = declarationOfficeIDNode.text().get();

        // 신청서 문서번호 추출
        pugi::xml_node idNode = doc.child("wco:Response").child("wco:Declaration").child("wco:ID");
        std::string id = idNode.text().get();

        // 신청서 문서구분 추출
        pugi::xml_node typeCodeDeclarationNode = doc.child("wco:Response").child("wco:Declaration").child("wco:TypeCode");
        std::string typeCodeDeclaration = typeCodeDeclarationNode.text().get();

        // 추출한 정보 출력
        std::cout << "접수통보일시: " << issueDateTime << std::endl;
        std::cout << "문서형태구분: " << typeCode << std::endl;
        std::cout << "접수 세관/과: " << declarationOfficeID << std::endl;
        std::cout << "신청서 문서번호: " << id << std::endl;
        std::cout << "신청서 문서구분: " << typeCodeDeclaration << std::endl;
    } else {
        std::cout << "XML 파일을 로드하는 중 오류가 발생했습니다." << std::endl;
        std::cout << "오류 내용: " << result.description() << std::endl;
    }

    return 0;
}