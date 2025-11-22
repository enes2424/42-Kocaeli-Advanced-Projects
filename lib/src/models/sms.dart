class SmsMessage {
  final int contactId;
  final String status;
  final String body;
  final String timestamp;

  SmsMessage({
    required this.contactId,
    required this.status,
    required this.body,
    required this.timestamp,
  });
}
