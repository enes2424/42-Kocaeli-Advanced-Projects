import 'package:flutter/material.dart';

import 'dart:async';

import '../models/app_words.dart';
import '../models/contact.dart';
import '../models/sms.dart';

import '../sms/sms_helper.dart';

import 'home_page.dart';

final GlobalKey<ChatPageState> chatPageKey = GlobalKey<ChatPageState>();

class ChatPage extends StatefulWidget {
  final Contact contact;

  const ChatPage({super.key, required this.contact});

  @override
  State<ChatPage> createState() => ChatPageState();
}

class ChatPageState extends State<ChatPage> {
  final TextEditingController _messageController = TextEditingController();
  final ScrollController _scrollController = ScrollController();
  bool _isSending = false;

  int get id => widget.contact.id;

  @override
  void dispose() {
    _messageController.dispose();
    _scrollController.dispose();
    super.dispose();
  }

  void safeSetState(VoidCallback fn) {
    if (mounted) {
      setState(fn);
    }
  }

  @override
  Widget build(BuildContext context) {
    final language = homePageKey.currentState!.appLanguage;

    final messages = widget.contact.messages.toList();

    final displayPhoneNumber = widget.contact.phoneNumber.startsWith('+90')
        ? "+90 ${widget.contact.phoneNumber.substring(3, 6)} ${widget.contact.phoneNumber.substring(6, 9)} "
              "${widget.contact.phoneNumber.substring(9, 11)} ${widget.contact.phoneNumber.substring(11)}"
        : widget.contact.phoneNumber;

    final displayName = widget.contact.nickname.isNotEmpty
        ? widget.contact.nickname
        : (widget.contact.name != widget.contact.phoneNumber
              ? "${widget.contact.name} ${widget.contact.surname}"
              : displayPhoneNumber);

    return Scaffold(
      appBar: AppBar(
        backgroundColor: homePageKey.currentState!.headerColor,
        foregroundColor: Colors.white,
        centerTitle: true,
        title: Row(
          mainAxisSize: MainAxisSize.min,
          children: [
            SizedBox(
              height: 40,
              width: 40,
              child: FittedBox(
                fit: BoxFit.contain,
                child: widget.contact.profileAvatar(),
              ),
            ),
            const SizedBox(width: 12),
            Flexible(
              child: Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                mainAxisSize: MainAxisSize.min,
                children: [
                  Text(
                    displayName,
                    style: const TextStyle(
                      fontSize: 16,
                      fontWeight: FontWeight.bold,
                    ),
                    overflow: TextOverflow.ellipsis,
                  ),
                  if (displayPhoneNumber != displayName)
                    Text(
                      displayPhoneNumber,
                      style: const TextStyle(
                        fontSize: 12,
                        color: Colors.white70,
                      ),
                      overflow: TextOverflow.ellipsis,
                    ),
                ],
              ),
            ),
          ],
        ),
      ),
      body: Column(
        children: [
          Expanded(
            child: messages.isEmpty
                ? Center(
                    child: Column(
                      mainAxisAlignment: MainAxisAlignment.center,
                      children: [
                        Icon(
                          Icons.chat_bubble_outline,
                          size: 64,
                          color: Colors.grey.shade400,
                        ),
                        const SizedBox(height: 16),
                        Text(
                          appWords[language]!['no_messages']!,
                          style: TextStyle(
                            fontSize: 18,
                            color: Colors.grey.shade600,
                          ),
                        ),
                        const SizedBox(height: 8),
                        Text(
                          appWords[language]!['start_chat']!,
                          style: TextStyle(
                            fontSize: 14,
                            color: Colors.grey.shade500,
                          ),
                        ),
                      ],
                    ),
                  )
                : Container(
                    decoration: BoxDecoration(color: Colors.grey.shade100),
                    child: ListView.builder(
                      controller: _scrollController,
                      padding: const EdgeInsets.symmetric(
                        horizontal: 12,
                        vertical: 16,
                      ),
                      itemCount: messages.length,
                      itemBuilder: (context, index) {
                        final message = messages[index];
                        final isSent = message.status == 'sent';
                        final datetime = DateTime.fromMillisecondsSinceEpoch(
                          int.parse(message.timestamp),
                        );

                        final showDateSeparator =
                            index == 0 ||
                            _shouldShowDateSeparator(
                              messages[index - 1],
                              message,
                            );

                        return Column(
                          children: [
                            if (showDateSeparator)
                              _buildDateSeparator(datetime),
                            _buildMessageBubble(message.body, isSent, datetime),
                          ],
                        );
                      },
                    ),
                  ),
          ),
          if (widget.contact.phoneNumber.startsWith('+90'))
            _buildMessageInput(),
        ],
      ),
    );
  }

  void scrollToBottomOnNewMessage() {
    WidgetsBinding.instance.addPostFrameCallback((_) {
      if (_scrollController.hasClients) {
        _scrollController.animateTo(
          _scrollController.position.maxScrollExtent,
          duration: const Duration(milliseconds: 300),
          curve: Curves.easeOut,
        );
      }
    });
  }

  Future<void> _sendMessage() async {
    if (_messageController.text.trim().isEmpty) return;

    final messageText = _messageController.text.trim();

    _messageController.clear();

    safeSetState(() {
      _isSending = true;
    });

    final timestamp = await SmsHelper.sendSms(
      context,
      widget.contact.id,
      widget.contact.phoneNumber,
      messageText,
    );

    if (timestamp == null) {
      safeSetState(() {
        _messageController.text = messageText;
        _isSending = false;
      });

      return;
    }

    final message = SmsMessage(
      contactId: widget.contact.id,
      status: 'sent',
      body: messageText,
      timestamp: timestamp,
    );

    widget.contact.addMessage(message);

    homePageKey.currentState?.safeSetState(() {});

    safeSetState(() {
      _isSending = false;
    });

    scrollToBottomOnNewMessage();
  }

  bool _shouldShowDateSeparator(SmsMessage previous, SmsMessage current) {
    final prevDate = DateTime.fromMillisecondsSinceEpoch(
      int.parse(previous.timestamp),
    );
    final currDate = DateTime.fromMillisecondsSinceEpoch(
      int.parse(current.timestamp),
    );

    return prevDate.day != currDate.day ||
        prevDate.month != currDate.month ||
        prevDate.year != currDate.year;
  }

  Widget _buildDateSeparator(DateTime date) {
    final now = DateTime.now();
    final today = DateTime(now.year, now.month, now.day);
    final yesterday = today.subtract(const Duration(days: 1));
    final messageDate = DateTime(date.year, date.month, date.day);

    final language = homePageKey.currentState!.appLanguage;

    String dateText;
    if (messageDate == today) {
      dateText = appWords[language]!['today']!;
    } else if (messageDate == yesterday) {
      dateText = appWords[language]!['yesterday']!;
    } else {
      dateText = '${date.day}/${date.month}/${date.year}';
    }

    return Padding(
      padding: const EdgeInsets.symmetric(vertical: 12),
      child: Container(
        padding: const EdgeInsets.symmetric(horizontal: 12, vertical: 6),
        decoration: BoxDecoration(
          color: Colors.grey.shade300,
          borderRadius: BorderRadius.circular(12),
        ),
        child: Text(
          dateText,
          style: TextStyle(
            fontSize: 12,
            color: Colors.grey.shade700,
            fontWeight: FontWeight.w500,
          ),
        ),
      ),
    );
  }

  Widget _buildMessageBubble(String text, bool isSent, DateTime datetime) {
    return Padding(
      padding: const EdgeInsets.only(bottom: 8),
      child: Align(
        alignment: isSent ? Alignment.centerRight : Alignment.centerLeft,
        child: Container(
          constraints: BoxConstraints(
            maxWidth: MediaQuery.of(context).size.width * 0.75,
          ),
          padding: const EdgeInsets.symmetric(horizontal: 14, vertical: 10),
          decoration: BoxDecoration(
            color: isSent
                ? homePageKey.currentState!.headerColor.withValues(
                    red: homePageKey.currentState!.headerColor.r * 0.9,
                    green: homePageKey.currentState!.headerColor.g * 0.9,
                    blue: homePageKey.currentState!.headerColor.b * 0.9,
                  )
                : Colors.white,
            borderRadius: BorderRadius.only(
              topLeft: const Radius.circular(18),
              topRight: const Radius.circular(18),
              bottomLeft: isSent
                  ? const Radius.circular(18)
                  : const Radius.circular(4),
              bottomRight: isSent
                  ? const Radius.circular(4)
                  : const Radius.circular(18),
            ),
            boxShadow: [
              BoxShadow(
                color: Colors.black.withAlpha(13),
                blurRadius: 5,
                offset: const Offset(0, 2),
              ),
            ],
          ),
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.end,
            children: [
              SelectableText(
                text,
                style: TextStyle(
                  color: isSent ? Colors.white : Colors.black87,
                  fontSize: 15,
                ),
              ),
              const SizedBox(height: 4),
              Text(
                '${datetime.hour}:${datetime.minute.toString().padLeft(2, '0')}',
                style: TextStyle(
                  color: isSent ? Colors.white70 : Colors.grey.shade600,
                  fontSize: 11,
                ),
              ),
            ],
          ),
        ),
      ),
    );
  }

  Widget _buildMessageInput() {
    final height = MediaQuery.of(context).size.height;
    final width = MediaQuery.of(context).size.width;

    return Container(
      decoration: BoxDecoration(
        color: Colors.white,
        boxShadow: [
          BoxShadow(
            color: Colors.black.withAlpha(13),
            blurRadius: 10,
            offset: const Offset(0, -2),
          ),
        ],
      ),
      padding: const EdgeInsets.symmetric(horizontal: 8, vertical: 8),
      child: SafeArea(
        child: Row(
          children: [
            Expanded(
              child: Container(
                decoration: BoxDecoration(
                  color: Colors.grey.shade100,
                  borderRadius: BorderRadius.circular(24),
                ),
                child: TextField(
                  controller: _messageController,
                  decoration: InputDecoration(
                    hintText:
                        appWords[homePageKey
                            .currentState!
                            .appLanguage]!['type_message']!,
                    hintStyle: TextStyle(color: Colors.grey.shade600),
                    border: InputBorder.none,
                    contentPadding: const EdgeInsets.symmetric(
                      horizontal: 20,
                      vertical: 10,
                    ),
                  ),
                  maxLength: 1000,
                  minLines: height > width ? 1 : null,
                  maxLines: height > width ? 5 : 1,
                  textCapitalization: TextCapitalization.sentences,
                  onSubmitted: (_) => _sendMessage(),
                ),
              ),
            ),
            const SizedBox(width: 8),
            Container(
              decoration: BoxDecoration(
                color: homePageKey.currentState!.headerColor.withValues(
                  red: homePageKey.currentState!.headerColor.r * 0.9,
                  green: homePageKey.currentState!.headerColor.g * 0.9,
                  blue: homePageKey.currentState!.headerColor.b * 0.9,
                ),
                shape: BoxShape.circle,
              ),
              child: IconButton(
                icon: _isSending
                    ? const SizedBox(
                        width: 20,
                        height: 20,
                        child: CircularProgressIndicator(
                          color: Colors.white,
                          strokeWidth: 2,
                        ),
                      )
                    : const Icon(Icons.send, color: Colors.white),
                onPressed: _isSending ? null : _sendMessage,
              ),
            ),
          ],
        ),
      ),
    );
  }
}
