
1. 관세청 전송 문서 : xml 형식(만들어져 있음)
2. xml 파일을 window 서버 특정폴더에 등록하면, 해당 소프트웨어가 해당 문서를 송신
3. 문서 송신 후, 결과 값이 xml 파일로 Return
4. Return 되는 xml 파일을 파싱하여 DB 등록

필요사항
1. xml 형식 파일
2. DB 연결정보 
3. 저장할 테이블 정보
4. 테스트 계정
5. 사용자인증방식 / 서버방식 선택 필요
6. 전자문서연계 요청양식 (테스트)



MinGW-w32를 설치(* 32bit 로 설치). 


GetSrvrInfo | 
Param 사용자ID, 사용자 문서함번호 | return URL 정보 | 관세청 서버의 URL 정보 조회

TrsmDocCscl | 
사용자ID,사용자 문서함번호, 문서코드, 제출번호, 송신XML파일명 | 정상/오류정보 | 통관관련 문서 송신

TrsmDocReqApre | 
사용자ID,사용자 문서함번호, 요건기관 문서함번호, 문서코드, 제출번호, | 송신XML파일명 | 정상/오류정보 요건신청문서 송신

RcpnDocLstCscl 
사용자ID,사용자 문서함번호  정상/오류정보, 수신 파일명 통관관련 목록 수신

RcpnDocLstReqApre 
사용자ID, 사용자 문서함번호 정상/오류정보, 수신 파일명 요건확인 목록 수신

RcpnDocCscl 
사용자ID, 사용자 문서함번호, 문서코드, XML문서번호 정상/오류정보, 수신 파일명 문서번호에 해당하는 XML파일 수신

RcpnDocReqApre 
사용자ID, 사용자 문서함번호, 문서코드, XML문서번호 정상/오류정보, 수신 파일명 문서번호에 해당하는 XML파일 수신

LoginSecuMdle 
사용자ID, 사용자 문서함번호 정상/오류정보 인증서 설정

LogoutSecuMdle 
없음 없음 인증서 해지

TrsmMltDoc 
사용자ID, 사용자 문서함번호 신청서 목록 파일 정상/오류정보 n건의 신청서를 다중문서 형 태로 송신 RcpnMltDoc 사용자ID, 사용자 문서함번호 정상/오류정보, 수신 문서 건수 n건의 통보서를 다중문서 형태로 수신



```관리자 CMD 창에서 실행 ```
reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\SessionManager\Environment" /v "PATH" /t REG_EXPAND_SZ /d "%PATH%;c:\KCSAPI4" /f


- GOVCBR936 : 통관목록 신고서 (파일 송신) => 시스템에서 파일 생성함. => itf_govcbr_936 , itf_govcbr_936_01
- GOVCBRR48 : 접수통보 (파일 수신) => 파일 수신시, 파싱해서 DB 등록 => itf_govcbr_r48 
- GOVCBRR20 : 사전검증 오류통보 (파일 수신) => 파일 수신시, 파싱해서 DB 등록 => itf_govcbr_r20 ,  itf_govcbr_r20_01 , itf_govcbr_r20_02  
- GOVCBR51R : 결과통보 (파일수신) => 파일 수신시, 파싱해서 DB 등록 => itf_govcbr_51r , itf_govcbr_51r_01

x509_srver.der 관세청 공용인증서 다운로드 파일(인증서가 필요)-> 불필요

- GOVCBR936 : 통관목록 신고서 (파일 송신) => 시스템에서 파일 생성함. 
- GOVCBRR48 : 접수통보 (파일 수신) => 파일 수신시, 파싱해서 DB 등록-> API전송으로 변경
- GOVCBRR20 : 사전검증 오류통보 (파일 수신) => 파일 수신시, 파싱해서 DB 등록 -> API전송으로 변경
- GOVCBR51R : 결과통보 (파일수신) => 파일 수신시, 파싱해서 DB 등록 -> API전송으로 변경

Program is running. Current count: 1
gsg.customs.go.kr:38120
C501 : 서식 검증 오류
com.mobicware.process.runtime.ProcessRuntimeException: Validate failure:XML 문서 구조는 동일한 엔티티 내에서 시작되고 끝나야 합니다.
-> 문서함 형식에 따른문제 (웹 방식 -> 사용자S/W 방식 변경)

1.5. API 제한사항
신고서 송신은 초당 20건 이하 / 분당 300건 이하로 전송 처리 한다.

[통보서 리스트 요청/요건확인 통보서 리스트요청] 시 
 1) 통보서 리스트가 있는 경우 업체에서 통보서 리스트 요청 후 리스트 수신완료 되면 재요청 가능하다.
 - 최초 (20xx.xx.xx 12:00:00) 에 리스트 요청 시 리스트 수신완료 되면 통보서요청 후 다시 리스트 요청 한다. 
 2) 통보서 리스트가 없는 경우 업체에서 통보서 리스트 요청 후 리스트 미수신 시 30초 후에 리스트 요청한다.
 - 최초 (20xx.xx.xx 12:00:00) 에 리스트 요청 시‘ C901’오류코드 [수신할 문서 없음(No Data)]를 받으면,
 30초 후 (20xx.xx.xx 12:00:30) 에 다시 리스트 요청 한다.