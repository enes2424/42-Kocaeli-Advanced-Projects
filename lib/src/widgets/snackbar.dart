import 'package:flutter/material.dart';
import 'dart:async';

String? _lastSnackbarMessage;
Timer? _snackbarResetTimer;

enum SnackBarType { success, info, error }

void showCustomSnackBar(
  BuildContext context,
  String message, {
  required SnackBarType type,
}) {
  if (!context.mounted) return;

  if (_lastSnackbarMessage == message) return;

  _lastSnackbarMessage = message;

  _snackbarResetTimer?.cancel();
  _snackbarResetTimer = Timer(const Duration(seconds: 4), () {
    _lastSnackbarMessage = null;
  });

  ScaffoldMessenger.of(context).hideCurrentSnackBar();

  final backgroundColor = type == SnackBarType.success
      ? Colors.green
      : type == SnackBarType.info
      ? Colors.white
      : Colors.red;

  ScaffoldMessenger.of(context).showSnackBar(
    SnackBar(
      content: Text(
        message,
        style: TextStyle(
          color: type == SnackBarType.info ? Colors.black : Colors.white,
          fontWeight: FontWeight.bold,
        ),
      ),
      backgroundColor: backgroundColor,
      duration: const Duration(seconds: 4),
      behavior: SnackBarBehavior.floating,
      shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(12)),
      margin: const EdgeInsets.all(16),
    ),
  );
}
